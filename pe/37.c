#include <stdio.h>
#include <string.h>

#define MAX_P  1000000
static char pf[MAX_P];
static int pt[MAX_P], nr;

static void sieve_p (int n)
{
    int i, j, k = 0, a = 4;

    pt[k++] = 2;
    pt[k++] = 3;
    for (i = 5; i < n; i+=(a^=6)) {
	if (pf[i]) continue;
	pt[k++] = i;
	for (j = i+(i<<1); j < n; j+=i<<1)
	    pf[j] = 1;
    }
    nr = k;
}

static void mark_p (int n)
{
    int i;

    memset(pf, 0, n);
    for (i = 0; i < nr; i++)
	pf[pt[i]] = 1;
}

static int is_truncatable (int n)
{
    int t = n, pdt = 1;

    while (t) {
	if (!pf[t]) return 0;
	t /= 10;
	pdt *= 10;
    }
    pdt /= 10;
    while (pdt > 1) {
	t = n % pdt;
	if (!pf[t]) return 0;
	pdt /= 10;
    }

    return 1;
}

int main (int argc, char *argv[])
{
    int i, sum = 0, c = 0;

    sieve_p(MAX_P);
    mark_p(MAX_P);

    for (i = 0; i < nr; i++)
	if (pt[i] > 10 && is_truncatable(pt[i])) {
	    sum += pt[i];
	    c++;
	}

    printf("%d %d\n", c, sum);

    return 0;
}
