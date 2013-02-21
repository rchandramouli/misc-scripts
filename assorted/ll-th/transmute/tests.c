#include <stdio.h>

#include "cchunk.h"

/* Try calling up the functions */
#define ptr2char(p, ptr) *(unsigned int *)(p) = (unsigned int)ptr

static void do_test_run(struct code_chunks *chunk)
{
	/* type-1 function */
	char df1[] = {
		0x64, 0x00, 0x00, 0x00,   /* 100 */
		'x', 0x00, 0x00, 0x00,    /* 'x' */
		0xC8, 0x00, 0x00, 0x00,   /* 200 */
	};

	/* type-2 function */
	char *name = "mygirl";
	char df2[] = {
		0x00, 0x00, 0x00, 0x00,   /* name */
		0x04, 0x00, 0x00, 0x00    /* 4 */
	};

	/* type-3 function */
	int fibo;
	char df3[] = {
		0x0f, 0x00, 0x00, 0x00,   /* 15 */
		0x00, 0x00, 0x00, 0x00    /* fibo */
	};

	char number[15];
	char df4[] = {
		0xde, 0xad, 0xbe, 0xef,
		0x00, 0x00, 0x00, 0x00   /* number */
	};

	char df5[] = { };

	ptr2char(df2, name);
	ptr2char((char *)df3 + 4, &fibo);
	ptr2char((char *)df4 + 4, number);

	call_function(chunk, FUNC_TYPE1, df1);
	call_function(chunk, FUNC_TYPE2, df2);

	call_function(chunk, FUNC_TYPE3, df3);
	printf("%s: fibo value returned: %d\n", __func__, fibo);

	call_function(chunk, FUNC_TYPE5, df5);

	call_function(chunk, FUNC_TYPE4, df4);
	printf("%s: original: %d, reversed: %s\n", __func__, *(int *)df4, number);
}

int main(int argc, char *argv[])
{
	struct code_chunks *code;

	code = get_code_chunks();
	if (code == NULL)
		return -1;

	dump_chunks(code);
	do_test_run(code);
	cleanup_chunk(code);

	return 0;
}
