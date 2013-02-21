#ifndef APP_H
#define APP_H

/*----------------------------------------------------------------------------
 * Clients, modelled as threads in the application. Some clients queue
 * up pages of data, some removes up the pages and some processes hash
 * value for those pages.
 *---------------------------------------------------------------------------*/
#define __thread_work                  /* well, nothing */

#define PAGE_SIZE                      4096

/* Page flags */
#define PF_PAGE_INVALID                0x80
#define PF_PAGE_FREE                   0x01
#define PF_PAGE_FULL                   0x02
#define PF_PAGE_INUSE                  0x04

/* Client work types */
#define CLIENT_WORK_QUEUE              0x01
#define CLIENT_WORK_DEQUEUE            0x02
#define CLIENT_WORK_HASH               0x03
#define CLIENT_WORK_SIGHANDLER         0x04

struct page {
	struct list list;

	char flags;
	unsigned int hashkey;
	int len;
	int pageno;
	char buffer[PAGE_SIZE];
};

#define CLIENT_THREAD_ATTACH_TIMEOUT             5
#define CLIENT_THREAD_ATTACH_RETRIES             5

struct client {
	struct list list;
	struct list *page;

	void *app_handle;

	char work;
	pthread_t thread;
	pthread_attr_t attr;

	/* Application plug tweaks */
	int attach_retries;
	int attach_timeout;
};

struct app {
	int nr_threads;
	char running:1;
	char shutdown:1;

	/* Signal handling thread */
	pthread_t sigthread;
	pthread_attr_t sigattr;

	pthread_mutex_t mutex;
	pthread_cond_t cond;

	/* Core data list */
	struct list pagelist_head;

	/* Clients */
	struct list client_head;
};

#define client_to_app(c)    ((struct app *)(c)->app_handle)

#define app_running(c)      (client_to_app(c)->running)
#define app_shutdown(c)     (client_to_app(c)->shutdown)
#define app_mutex(c)        (client_to_app(c)->mutex)
#define app_cond(c)         (client_to_app(c)->cond)

#define lock_client(c)      pthread_mutex_lock(&app_mutex(c))
#define unlock_client(c)    pthread_mutex_unlock(&app_mutex(c))

#define for_each_client(a, c)          \
        for_each_node_in_list(&(a)->client_head, c)

#define for_each_client_safe(a, c, t)          \
        for_each_node_in_list_safe(&(a)->client_head, c, t)

#endif /* APP_H */
