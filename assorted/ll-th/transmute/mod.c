#include <stdio.h>

#include "cchunk.h"

/* Mutable functions and definitions */
#if 0
static int __mutable fun1(int x, int sym, int y)
{
	printf("%s: x = %d, sym = '%c', y = %d\n", __func__, x, sym & 0xff, y);
	return 0;
}

static int __mutable fun2(char *name, int order)
{
	printf("%s: name: \"%s\", order: %d\n", __func__, name, order);
	return 0;
}

static void __mutable compute_fibo(int n, int *x)
{
	int f1 = 0, f2 = 1, f3 = 1, t = n;

	while (t--) {
		f1 = f2;
		f2 = f3;
		f3 = f1+f2;
	}

	*x = f1;

	printf("%s: fibo(%d) = %d stored @ %p\n", __func__, n, f1, x);
}
#endif
static void __mutable recurse(unsigned int n, char *x)
{
	if (n == 0) {
		*x = 0;
		return;
	}

	*x = (n % 10)+'0';
	recurse(n/10, x+1);

	printf("%s: reverse = %s\n", __func__, x);
}

static void __mutable fun_x(void)
{
	printf("%s: got called\n", __func__);
}

static void __mutable chain(void)
{
	printf("%s: now would call fun_x\n", __func__);
	fun_x();
	printf("%s: returned from fun_x\n", __func__);
}

static unsigned long __mute_array fptrs[] = {
#if 0
	FUNC_TYPE1, (unsigned long)&fun1,
	FUNC_TYPE2, (unsigned long)&fun2,
	FUNC_TYPE3, (unsigned long)&compute_fibo,
#endif
	FUNC_TYPE4, (unsigned long)&recurse,
	FUNC_TYPE5, (unsigned long)&chain,
};
