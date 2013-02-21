/* Process manager. */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <error.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "list.h"
#include "pm.h"

#define PM_CONFIG_FILE          "proc.rc"

#define false        1
#define true         0

/* Globals */
struct pm_private {
	int nprocs;

	struct list head;
	struct list gc;
};

static struct pm_private pm_proc_list;

static void pm_init(void)
{
	list_head_init(&pm_proc_list.head);
	list_head_init(&pm_proc_list.gc);

	signal(SIGCHLD, SIG_DFL);
}

/* Allocate a free socket address */
#define MAX_SOCKET_LEN   4
static unsigned char addr[MAX_SOCKET_LEN], idx = 0;

static int pm_alloc_sockaddr(unsigned char *saddr, int len)
{
	int i;

	if (!addr[0]) {
		for (i = 0; i < MAX_SOCKET_LEN; i++)
			addr[i] = 1;
	}

	if (len > MAX_SOCKET_LEN)
		return false;

	addr[idx]++;
	while (idx < MAX_SOCKET_LEN && !addr[idx]) {
		addr[idx] = 1;
		idx++;
		addr[idx]++;
	}
	if (idx >= MAX_SOCKET_LEN) {
		return false;
	}
	idx = 0;
	for (i = 0; i < len; i++) {
		saddr[i] = addr[i];
	}

	return true;
}

static sigjmp_buf jmp_child;
static void parent_signal_handler(int sig)
{
	siglongjmp(jmp_child, 1);
}

static void wait_for_parent_and_exec(struct proc_info *proc)
{
	int ret;

	if (signal(SIGUSR1, parent_signal_handler) < 0) {
		perror("signal");
		return;
	}
	ret = sigsetjmp(jmp_child, 0);
	while (!ret) {
		sched_yield();
	}

	/* exec the process */
	printf("%s: starting process (%d) '%s'\n", __func__, proc->pid,
	       proc->paths->prog);
	if (execve(proc->paths->prog, proc->paths->args, NULL) < 0) {
		perror("execve");
		return;
	}
}

static inline void notify_child(struct proc_info *proc)
{
	if (proc->pid)
		kill(proc->pid, SIGUSR1);
}

/* Wake up processes one by one */
static int pm_spawn_process(struct proc_info *proc)
{
	int pid = 0;
	struct stat st;

	/* validate paths */
	if (stat(proc->paths->prog, &st)) {
		perror("stat");
		return -1;
	}

	if ((pid = fork()) > 0) { /* parent */
		proc->pid = pid;
		printf("%s: spawned child PID: %d\n", __func__, proc->pid);

	} else if (!pid) { /* child */
		proc->pid = getpid();
		wait_for_parent_and_exec(proc);
	}

	return 0;
}

static void pm_create_processes(void)
{
	struct list *node;
	struct proc_info *proc;
	int res;

	for_each_node(&pm_proc_list.head, node) {
		proc = to_proc_info(node);

		res = pm_spawn_process(proc);
		if (res) {
			fprintf(stderr, "%s: failed to spawn process '%s', "
				"status = %d\n", __func__, proc->procname, res);
		}
	}

	/* Notify all processes to start */
	for_each_node(&pm_proc_list.head, node) {
		proc = to_proc_info(node);
		notify_child(proc);
	}
}

/* Parse process information and add them up to the proc list */
static int parse_procrc(char *rcfile)
{
	FILE *fp;
	struct proc_info *p;

	if ((fp = fopen(rcfile, "r")) == NULL) {
		perror("fopen");
		return false;
	}

	while ((p = parse_proc_info(fp))) {
		pm_proc_list.nprocs++;

		/* get unused address for communication for the process */
		pm_alloc_sockaddr(p->addr, sizeof(p->addr));

		list_add_node(&pm_proc_list.head, &p->proc_list);
	}

	return true;
}

static void dump_all_procs(void)
{
	struct list *node;
	struct proc_info *proc;

	for_each_node(&pm_proc_list.head, node) {
		proc = to_proc_info(node);

		printf("****** PROCESS INFO: (%s) ******\n",
		       proc->procname);
		printf("address: %d.%d.%d.%d\n",
		       proc->addr[0], proc->addr[1], proc->addr[2], proc->addr[3]);
		printf("heartbeat: %s, malloc_trace: %s\n",
		       ((proc->heartbeat) ? "yes": "no"),
		       ((proc->malloc_trace) ? "yes": "no"));
		printf("exe path: %s\n\n", proc->paths->prog);
	}
}

static void pm_mainloop(void)
{
	struct list *node = NULL;
	struct proc_info *proc;

	/* Wait for all processes */
	node = &pm_proc_list.head;

	while (1) {
		if (node->next == node) {
			sched_yield();
			printf("%s: no more process to reap!\n", __func__);
			return;
		}

		/* wait for heartbeat */
		sleep(1);

		node = node->next;
		proc = to_proc_info(node);

		/* Wait a quanta */
		sched_yield();
	}
}

int main(int argc, char *argv[])
{
	pm_init();
	parse_procrc(PM_CONFIG_FILE);
	dump_all_procs();
	pm_create_processes();
	pm_mainloop();

	return 0;
}
