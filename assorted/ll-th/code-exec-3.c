#include <stdio.h>

struct data_fun1 {
	int x;
	char sym;
	int y;
};
static int fun1(void *datum)
{
	struct data_fun1 *df1 = datum;

	printf("x = %d, sym = '%c', y = %d\n", df1->x, df1->sym, df1->y);
	return 0;
}

struct data_fun2 {
	char *name;
	int order;
};
static int fun2(void *datum)
{
	struct data_fun2 *df2 = datum;

	printf("name: \"%s\", order: %d\n", df2->name, df2->order);
	return 0;
}

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

	(*f[0])(df1);
	(*f[1])(df2);

	return 0; 
}
