#include <stdio.h>

#define MAX_P   1000000
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

static int get_nr_factors (int n)
{
    int i, c = 0;

    for (i = 0; 2*pt[i] <= n; i++) {
	if ((n % pt[i]) == 0)
	    c++;
    }
    return c;
}

int main (int argc, char *argv[])
{
    int n, k, m, f, d[10];

    scanf("%d", &k);

    sieve_p(MAX_P);

    for (n = 1; n < MAX_P; ) {
	for (f = m = 0; m < k; m++) {
	    d[m] = get_nr_factors(n+m);
	    if (d[m] != k) {
		n+= m+1;
		f = 1;
		break;
	    }
	}
	if (!f) {
	    printf("%d\n", n);
	    break;
	}
    }

    return 0;
}
