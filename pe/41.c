#include <stdio.h>
#include <math.h>

#define MAX_P  1000000

static int  pt[MAX_P], nr_p;
static char pf[MAX_P];

static void sieve_p (void)
{
    int i, j, k = 0;

    pt[k++] = 2;
    for (i = 3; i < MAX_P; i += 2) {
	if (pf[i] == 1) continue;
	pt[k++] = i;

	for (j = i+(i<<1); j < MAX_P; j += (i<<1))
	    pf[j] = 1;
    }
    nr_p = k;
}

static int is_prime (int n)
{
    int mx = sqrt(n), i;

    for (i = 0; i < nr_p && pt[i] <= mx; i++) {
	if ((n % pt[i]) == 0)
	    return 0;
    }

    return 1;
}

static int nr[7] = { 1, 2, 3, 4, 5, 6, 7 }, mx = 0;

static void check_if_prime (void)
{
    int n;

    n = ((((((nr[0])*10+nr[1])*10+nr[2])*10+nr[3])*10+nr[4])*10+nr[5])*10+nr[6];
    if (is_prime(n)) {
	if (mx < n) {
	    mx = n;
	}
	printf("%d\n", n);
    }
}

static void next_perm (int n)
{
    int i, t;

    if (n == 1) {
	check_if_prime();
	return;
    }

    for (i = 0; i < n-1; i++) {
	t = nr[i];
	nr[i] = nr[n-1];
	nr[n-1] = t;

	next_perm(n-1);

	t = nr[i];
	nr[i] = nr[n-1];
	nr[n-1] = t;
    }
}

int main (int argc, char *argv[])
{
    sieve_p();
    next_perm(7);

    printf("\nmax = %d\n", mx);

    return 0;
}
