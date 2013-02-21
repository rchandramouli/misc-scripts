#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>

#define __USE_GNU
#include <pthread.h>

#include "ll.h"
#include "random.h"
#include "app.h"

static inline void fill_timeout(struct timespec *ts, int timeout)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	ts->tv_sec = tv.tv_sec + timeout;
	ts->tv_nsec = tv.tv_usec * 1000;
}

/* We wait till the application has initialized all threads. But not
 * much more than (timeout * retries) seconds per thread!
 */
static int wait_for_app_to_start(struct client *cli)
{
	struct timespec ts;
	int ret = 0, timeout, retries;

	lock_client(cli);
	retries = cli->attach_retries;
	timeout = cli->attach_timeout;

	fill_timeout(&ts, timeout);
	while (!app_running(cli) && retries) {
		ret = pthread_cond_timedwait(&app_cond(cli), &app_mutex(cli), &ts);

		if (ret == ETIMEDOUT) {
			retries--;
			fill_timeout(&ts, timeout);
		}
	}
	unlock_client(cli);

	return (ret) ? -1: 0;
}

/* THREAD-WORK 1: Queue up a random page */
static void __thread_work do_work_queue(struct client *cli)
{
	struct page *page;
	static unsigned int pageno = 1;

	while (!app_shutdown(cli)) {
		page = malloc(sizeof(struct page));
		if (!page) {
			perror("malloc");
			pthread_yield();
			continue;
		}

		page->flags = 0x00;
		page->hashkey = 0x00;
		page->len = PAGE_SIZE;

		get_random_bytes(page->buffer, page->len);
		page->flags = PF_PAGE_FULL;

		/* Queue the page up for the other thread to process. */
		lock_client(cli);
		page->pageno = pageno++;
		list_add_link(cli->page, &page->list);
		unlock_client(cli);
	}
}

static void page_compute_hash(struct page *page)
{
	int i;
	unsigned int hash = 0, r = 0;

	for (i = 0; i < page->len; i++) {
		hash ^= ((unsigned int)page->buffer[i] & 0xff) << r;
		r = (r + 1) % 24;
	}

	page->hashkey = hash;
}

/* Lock before calling */
static inline struct page *__pick_a_page(struct client *cli, int flags)
{
	struct page *page;
	struct list *iter;

	for_each_node_in_list(cli->page, iter) {
		page = container_of(iter, struct page, list);
		if ((page->flags & flags) == flags) {
			return page;
		}
	}

	return NULL;
}

/* THREAD-WORK 2: Compute HASH value of the text in buffer */
static void __thread_work do_work_hash(struct client *cli)
{
	struct page *page;

	while (!app_shutdown(cli)) {
		/* Pick a page to process */
		lock_client(cli);
		page = __pick_a_page(cli, PF_PAGE_FULL);

		if (page) {
			list_remove_link(&page->list);
			unlock_client(cli);
		} else {
			unlock_client(cli);
			pthread_yield();
			continue;
		}

		page_compute_hash(page);
		page->flags = PF_PAGE_FREE;

		lock_client(cli);
		list_add_link(cli->page, &page->list);
		unlock_client(cli);
	}
}

/* THREAD-WORK 3: Remove the page from the list */
static void __thread_work do_work_dequeue(struct client *cli)
{
	struct page *page;

	while (!app_shutdown(cli)) {
		lock_client(cli);
		page = __pick_a_page(cli, PF_PAGE_FREE);

		if (page) {
			list_remove_link(&page->list);
			unlock_client(cli);
		} else {
			unlock_client(cli);
			pthread_yield();
			continue;
		}

		printf("%s: page ID %8u, hash value: %08x\n", __func__,
		       page->pageno, page->hashkey);
		free(page);
	}
}

/* THREAD-WORK 4: Idle thread: shhh, don't disturb! */
static void __thread_work do_work_idle(struct client *cli)
{
	while (!app_shutdown(cli)) {
		pthread_yield();
	}
}

/* The actual thread handlers. We have two kinds of client. One, a
 * worker client and another a signal handler client. The worker
 * clients are invoked from this thread_runner routine.
 */

/* Trampoline to initiate the work */
static void *thread_runner(void *priv)
{
	struct client *cli = priv;

	if (wait_for_app_to_start(cli) < 0) {
		printf("thread %lu: timeout waiting for application to start, "
		       "aborting.\n", pthread_self());
		return NULL;
	}

	switch (cli->work) {
	case CLIENT_WORK_QUEUE:
		do_work_queue(cli);
		break;

	case CLIENT_WORK_DEQUEUE:
		do_work_dequeue(cli);
		break;

	case CLIENT_WORK_HASH:
		do_work_hash(cli);
		break;

	default:
		do_work_idle(cli);
	}

	return NULL;
}

/* Handle all signals despatched to this process. */
static void *signal_handler(void *priv)
{
	sigset_t sigset;
	int sig;

	while (1) {
		sigfillset(&sigset);

		/* Block till the process gets a signal */
		sigwait(&sigset, &sig);

		if (sig == SIGQUIT || sig == SIGINT) {
			printf("\n%s: INTR! aborting application\n", __func__);
			return (void *)1;
		}
	}

	return NULL;
}

/* Add/Remove clients in the application */
static struct client *app_add_new_client(struct app *app, char work)
{
	struct client *cli;

	cli = malloc(sizeof(struct client));
	if (!cli) {
		perror("malloc");
		return NULL;
	}

	list_init(&cli->list);
	cli->app_handle = app;
	cli->page = &app->pagelist_head;
	cli->work = work;
	cli->attach_retries = CLIENT_THREAD_ATTACH_RETRIES;
	cli->attach_timeout = CLIENT_THREAD_ATTACH_TIMEOUT;

	list_add_link(&app->client_head, &cli->list);

	return cli;
}

/* So bad, do not call this before stopping the thread, if initialized
 * in the client! Else, all hell would break loose!
 */
static void app_remove_client(struct app *app, struct client *cli)
{
	list_remove_link(&cli->list);
	free(cli);
}

/* Thread mainloop! Actually, a threaded application does not require
 * a mainloop, but requires a common cancellation function if the
 * threads are JOINABLE, and by default, they are so! So, we got to
 * gracefully stop the threads and then join them up. The 'shutdown'
 * flag in the app says the application is going for a shutdown and
 * thus makes the threads to finish off their jobs!
 */
static void app_wait_on_threads(struct app *app)
{
	int status;
	struct client *cli;
	struct page *page;
	struct list *iter, *temp;

	/* We assume that our first client is a signal handler */
	cli = container_of(app->client_head.next, struct client, list);
	if (app->client_head.next != &app->client_head &&
	    cli->work == CLIENT_WORK_SIGHANDLER) {
		pthread_join(cli->thread, (void **)&status);
	}

	for_each_client_safe(app, iter, temp) {
		cli = container_of(iter, struct client, list);

		if (status == 1) {
			app->shutdown = 1;
			pthread_join(cli->thread, NULL);
		}
		app_remove_client(app, cli);
	}

	/* Remove pages if any */
	for_each_node_in_list_safe(&app->pagelist_head, iter, temp) {
		page = container_of(iter, struct page, list);
		list_remove_link(iter);
		free(page);
	}
}

/* Here is a small trick. We move the signals handled by the process
 * to _blocked_ so that all other threads created after the
 * pthread_sigmask() would have those signals as blocked too! But, the
 * sigwait() called up in the signal_handler() would wait by
 * unblocking the blocked signals and handle them! Whereas, the kernel
 * when dispatching the signal, would not dispatch to the thread that
 * has blocked the signals. So, this is kind of, streamlining the
 * async signals to a single thread!
 */
static int app_create_signal_thread(struct app *app)
{
	sigset_t sigset;
	struct client *cli;

	cli = app_add_new_client(app, CLIENT_WORK_SIGHANDLER);
	if (!cli) {
		return -1;
	}

	/* Save the signal mask and block all signals */
	sigfillset(&sigset);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);

	/* Create signal thread */
	if (pthread_create(&cli->thread, &cli->attr, signal_handler, cli) < 0) {
		perror("pthread_create");
		app_remove_client(app, cli);
		return -1;
	}

	return 0;
}

/* Initialize the signal thread and worker threads */
static int app_init_threads(struct app *app, int n)
{
	int i;

	app->running = 0;
	app->shutdown = 0;
	app->nr_threads = n;

	/* Initialize page/clients list */
	list_init(&app->pagelist_head);
	list_init(&app->client_head);

	pthread_mutex_init(&app->mutex, NULL);
	pthread_cond_init(&app->cond, NULL);

	if (app_create_signal_thread(app) < 0)
		return -1;

	/* Initialize clients */
	for (i = 0; i < n; i++) {
		struct client *cli;
		char work = CLIENT_WORK_HASH;

		if (i % 3 == 1)
			work = CLIENT_WORK_QUEUE;
		else if (i % 3 == 2)
			work = CLIENT_WORK_DEQUEUE;

		cli = app_add_new_client(app, work);
		if (!cli)
			break;

		pthread_attr_init(&cli->attr);
		if (pthread_create(&cli->thread, &cli->attr,
				   thread_runner, cli) < 0) {
			perror("pthread_create");
			app_remove_client(app, cli);
			app->nr_threads = i+1;
			break;
		}

		sleep(1);
	}

	app->running = 1;

	/* Broadcast all threads */
	pthread_cond_broadcast(&app->cond);

	return app->nr_threads;
}

#define MAX_THREADS     16

int main(int argc, char *argv[])
{
	int n;
	struct app app_instance;

	scanf("%d", &n);

	if (n <= 0 || n > MAX_THREADS)
		return -1;

	init_random();

	if (app_init_threads(&app_instance, n) < 0)
		return -1;

	app_wait_on_threads(&app_instance);

	fini_random();

	return 0;
}
