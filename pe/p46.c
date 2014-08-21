#include <stdio.h>
#include <string.h>
#include <math.h>

#define PMAX    100000
static char pf[PMAX];
static int pt[PMAX], nr_p;

static void sieve_p (int n)
{
    int i, j, k, a = 4;

    pt[0] = 2;
    pt[1] = 3;
    for (i = 5, k = 2; i < n; i+=(a^=6)) {
	if (pf[i]) continue;
	pt[k++] = i;
	for (j = i+(i<<2); j < n; j+=i<<1)
	    pf[j] = 1;
    }
    nr_p = k;
}

static inline int is_valid_n (int n)
{
    int p, s, r;

    for (p = 1; pt[p] <= n; p++) {
	if (pt[p] == n) return 1;

	s = (n-pt[p])>>1;
	r = sqrt(s);
	if (r*r == s) return 1;
    }
    return 0;
}

int main (int argc, char *argv[])
{
    int i;

    sieve_p(PMAX);

    for (i = 3; i < PMAX; i += 2) {
	if (!is_valid_n(i)) {
	    printf("%d\n", i);
	    break;
	}
    }

    return 0;
}
