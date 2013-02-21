#ifndef PM_H
#define PM_H

#define MAX_PROCNAME_LEN         128
#define MAX_PROCARGS_LEN         1024
#define MAX_PROCPATH_LEN         1024
#define MAX_PROCADDR_LEN         4

struct proc_paths_info {
	char instream[MAX_PROCPATH_LEN];
	char outstream[MAX_PROCPATH_LEN];
	char errstream[MAX_PROCPATH_LEN];
	char prog[MAX_PROCPATH_LEN];
	char *args[MAX_PROCARGS_LEN];
};

struct proc_info {
	int pid;
	int mem_limit;
	int core_limit;
	int fork_limit;
	int heartbeat;
	int malloc_trace;
	int infd;
	int outfd;
	int errfd;

	char procname[MAX_PROCNAME_LEN];
	unsigned char addr[MAX_PROCADDR_LEN];

	struct proc_paths_info *paths;
	struct list proc_list;
};

#define to_proc_info(l)    container_of(l, struct proc_info, proc_list)

extern void *x_malloc(size_t size);
extern void *x_calloc(size_t size, int nmemb);
extern struct proc_info *alloc_proc_info(void);
extern void free_proc_info(struct proc_info *proc);

extern struct proc_info *parse_proc_info(FILE *fp);

#endif
