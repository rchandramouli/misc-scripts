#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

static void trim_test(void)
{
  char *strs[] = {
    "no trim",
    "right trim            ",
    "           left trim",
    "     left-right trim      ",
    "         A       ",
    "A          ",
    "         A",
    "",
    "              ",
  }, p[64];
  int i, len;

  printf("\t\t *** TRIM TEST ***\n");
  for (i = 0; i < sizeof(strs)/sizeof(char *); i++) {
    len = strlen(strs[i]);
    strncpy(p, strs[i], len+1);

    printf("before: [%s]\n", p);
    trim_string(p, len);
    printf("after:  [%s]\n\n", p);
  }
}

static void get_param_test(void)
{
  char *lines[] = {
    "# process-name:path:mem-limit:core-limit:fork-limit:heartbeat:malloc-trace:in-file:out-file:error-file:args-list\n",
    "\n",
    "myproc1:/home/rchandramouli/scripts/proc/test/myproc1::::yes:yes::::list of args\n",
    "myproc2:/home/rchandramouli/scripts/proc/test/myproc2::::yes:no::::\n",
    "\n",
  };
  char buf[1024], param[1024], *p;
  int i, len, arg;

  printf("\t\t *** GET_PARAM_TEST ***\n");
  for (i = 0; i < sizeof(lines)/sizeof(char *); i++) {
    len = strlen(lines[i]);
    strncpy(buf, lines[i], len+1);

    printf("PARAM LINE: %s\n", lines[i]);
    p = buf;
    arg = 0;
    while ((p = get_param_value(p, param))) {
      printf("\t(arg %d): %s\n", ++arg, (param[0]) ? param: "<empty>");
    }
    printf("Total params = %d\n", arg);
  }
}

int main(int argc, char *argv[])
{
  trim_test();
  get_param_test();

  return 0;
}


