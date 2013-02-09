#include <stdio.h>

static unsigned int dgt[] = {
    0,
    9*1*1,
    9*2*10,
    9*3*100,
    9*4*1000,
    9*5*10000,
    9*6*100000,
    9*7*1000000,
    9*8*10000000,
    0,
};

static int get_nth_digit (int n)
{
    int t = 1, r = 1, pdt = 1, m;

    for (; dgt[r]; r++) {
	if (dgt[r] >= n) break;
	pdt *= 10;
	n -= dgt[r];
    }
    if (!dgt[r]) return 0;

    m = r - ((n - 1) % r) - 1;
    t = pdt + ((n - 1) / r);

    while (m--) {
	t /= 10;
    }
    return (t % 10);
}

#define P(n)  printf("d%d = %d\n", n, get_nth_digit(n))

int main (int argc, char *argv[])
{
    int n;

    P(1);
    P(10);
    P(100);
    P(1000);
    P(10000);
    P(100000);
    P(1000000);

    while (scanf("%dn", &n) == 1)
	P(n);

    return 0;
}
