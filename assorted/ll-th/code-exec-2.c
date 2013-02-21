#include <stdio.h>

static int fun1(int x, char sym, int y)
{
	printf("x = %d, sym = '%c', y = %d\n", x, sym, y);
	return 0;
}

static int fun2(char *name, int order)
{
	printf("name: \"%s\", order: %d\n", name, order);
	return 0;
}

#define MAX_PARAM_LENGTH     1024

struct magic {
	char magic[MAX_PARAM_LENGTH];
};

static int (*f[2])();

#define ptr2char(p, ptr) *(unsigned int *)p = (unsigned int)ptr

int main(int argc, char *argv[])
{
	char df1[] = {
		0x64, 0x00, 0x00, 0x00,   /* 100 */
		'x', 0x00, 0x00, 0x00,    /* 'x' */
		0xC8, 0x00, 0x00, 0x00,   /* 200 */
	};

	char *name = "mygirl";
	char df2[] = {
		0x00, 0x00, 0x00, 0x00,   /* name */
		0x04, 0x00, 0x00, 0x00    /* 4 */
	};

	ptr2char(df2, name);

	f[0] = (int (*)())fun1;
	f[1] = (int (*)())fun2;

	(*f[0])(*(struct magic *)df1);
	(*f[1])(*(struct magic *)df2);

	return 0; 
}
