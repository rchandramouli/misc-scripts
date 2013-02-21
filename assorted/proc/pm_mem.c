/* Memory allocation routines */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>

#include "list.h"
#include "pm.h"

/* alloc or abort */
inline void *x_calloc(size_t size, int nmemb)
{
	void *p = calloc(size, nmemb);

	if (!p) {
		perror("calloc");
		exit(-1);
	}

	return p;
}

inline void *x_malloc(size_t size)
{
	void *p = malloc(size);

	if (!p) {
		perror("malloc");
		exit(-1);
	}

	return p;
}

struct proc_info *alloc_proc_info(void)
{
	struct proc_info *p;

	p = x_calloc(sizeof(struct proc_info) + sizeof(struct proc_paths_info), 1);
	p->paths = (struct proc_paths_info *)
		(((unsigned char *)p) + sizeof(struct proc_info));

	return p;
}

void free_proc_info(struct proc_info *proc)
{
	free(proc);
}
