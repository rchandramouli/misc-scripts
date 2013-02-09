#include <stdio.h>
#include <string.h>

#define MAX_P    1000000
static char pf[MAX_P], pf1[MAX_P];
static int  pt[MAX_P], nr;

static void sieve_p (int n)
{
    int i, j, k = 0, a = 4;

    pt[k++] = 2;
    pt[k++] = 3;
    for (i = 5; i < n; i += (a ^= 6)) {
	if (pf[i]) continue;
	pt[k++] = i;
	for (j = i+(i<<1); j < n; j+=(i<<1))
	    pf[j] = 1;
    }
    nr = k;
}

static void mark_p (int n)
{
    int i;

    memset(pf, 0x0, n);
    for (i = 0; i < nr; i++) {
	pf[pt[i]] = 1;
    }
}

static int mark_rot_prime (int n)
{
    int pdt = 1;
    int t = n, c = 0, k = 0, i;

    while (t) {
	t /= 10;
	pdt *= 10;
	c++;
    }
    pdt /= 10;

    for (t = n, i = 0; i < c; i++) {
	if (pf1[t] == 0) k++;
	pf1[t] = 1;

	if (pf[t] == 0)
	    return 0;
	t = (t / pdt) + (t % pdt)*10;
    }
    return k;
}

static void count_rot_primes (void)
{
    int i, j, c;

    for (i = c = 0; i < nr; i++) {
	if (pf1[pt[i]]) continue;

	if ((j = mark_rot_prime(pt[i]))) {
	    printf("%d ", pt[i]);
	    c += j;
	}
    }

    printf("\n\n%d\n", c);
}

int main (int argc, char *argv[])
{
    int n;

    scanf("%d", &n);

    sieve_p(n);
    mark_p(n);
    count_rot_primes();

    return 0;
}
