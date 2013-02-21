#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "list.h"
#include "pm.h"

#define MAX_BUF_LENGTH    4096

static int get_trim_length(char *line, int len, int *offt)
{
	int i = 0, j = len-1;

	while (i <= j && isspace(line[i]))
		i++;
	if (i > j) return 0;

	while (j >= i && isspace(line[j]))
		j--;

	if (offt) {
		*offt = i;
	}

	return (j-i+1);
}

static int trim_string(char *line, int len)
{
	int i, j, k;

	k = get_trim_length(line, len, &i);

	for (j = 0; j < k; j++)
		line[j] = line[j+i];
	line[j] = 0;

	return k-1;
}

static char *get_param_value(char *line, char *param)
{
	int len, toks;

	if (line == NULL || *line == '\0')
		return NULL;

	toks = sscanf(line, "%[^:]s", param);
	if (!toks) {
		param[0] = 0;
		len = 0;
	} else {
		len = strlen(param);
		trim_string(param, len);
	}

	return (line + len + ((line[len] == ':') ? 1: 0));
}

static int fill_proc_info(struct proc_info *proc, char *line)
{
	char param[MAX_BUF_LENGTH];
	int fill = 0;

#define __get_param(line, param)					\
	((line = get_param_value(line, param)) && (param[0]) && (fill |= 1))

	/* proc-name */
	if (__get_param(line, param))
		strncpy(proc->procname, param, MAX_PROCNAME_LEN);

	/* exe path */
	if (__get_param(line, param))
		strncpy(proc->paths->prog, param, MAX_PROCPATH_LEN);

	/* mem-limit */
	if (__get_param(line, param))
		sscanf(param, "%d", &proc->mem_limit);

	/* core-limit */
	if (__get_param(line, param))
		sscanf(param, "%d", &proc->core_limit);

	/* fork-limit */
	if (__get_param(line, param))
		sscanf(param, "%d", &proc->fork_limit);

	/* heartbeat */
	if (__get_param(line, param) && !strcasecmp(param, "yes"))
		proc->heartbeat = 1;

	/* malloc-trace */
	if (__get_param(line, param) && !strcasecmp(param, "yes"))
		proc->malloc_trace = 1;

	/* in/out/err-redir */
	if (__get_param(line, param))
		strncpy(proc->paths->instream, param, MAX_PROCPATH_LEN);
	if (__get_param(line, param))
		strncpy(proc->paths->outstream, param, MAX_PROCPATH_LEN);
	if (__get_param(line, param))
		strncpy(proc->paths->errstream, param, MAX_PROCPATH_LEN);

	/* parse arguments: TODO */
#undef __get_param

	return fill;
}

struct proc_info *parse_proc_info(FILE *fp)
{
	struct proc_info *proc = NULL;
	char *line = NULL;
	size_t len, sz = 0;
	int i, valid = 0;

	/* Strip comments & blank lines */
	while (1) {
		if ((len = getline(&line, &sz, fp)) == -1)
			break;

		i = 0;
		while (i < len && isspace(line[i++]));
		if (i >= len || line[i-1] == '#')
			continue;

		if (!proc) {
			if ((proc = alloc_proc_info()) == NULL)
				break;
		}
		if ((valid = fill_proc_info(proc, line)))
			break;
	}

	if (line)
		free(line);
	if (!valid && proc) {
		free_proc_info(proc);
		proc = NULL;
	}

	return proc;
}
