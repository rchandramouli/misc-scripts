#include <stdio.h>

static int dbmp, num;

static inline int is_pandigital (void)
{
    return ((dbmp & 0x3fe) == 0x3fe);
}

static inline void init_bmp (void)
{
    dbmp = num = 0;
}

static inline int add_to_bmp (int n)
{
    int x, t = n, p = 1;

    while (n) {
	x = 1 << (n % 10);
	if (x != 1) {
	    if (dbmp & x)
		return 0;
	    dbmp |= x;
	    p *= 10;
	} else {
	    return 0;
	}
	n /= 10;
    }
    num = (num * p) + t;

    return 1;
}

int main (int argc, char *argv[])
{
    int n, d, c, x, mx = 0;

    for (d = 1, c = 0; d < 100000; d++) {
	init_bmp();
	for (n = 1; n < 10; n++) {
	    x = add_to_bmp(n*d);
	    if (!x) break;
	    if (is_pandigital()) break;
	}
	if (x && is_pandigital()) {
	    c++;
	    printf("d = %d, n = %d, num = %d\n", d, n, num);

	    if (mx < num) {
		mx = num;
	    }
	}
    }
    printf("%d, %d\n", c, mx);

    return 0;
}
