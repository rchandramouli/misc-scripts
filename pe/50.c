#include <stdio.h>
#include <math.h>

#define MAX_P   1000000
static char pf[MAX_P];
static int pt[MAX_P], nr;

static void sieve_p (int n)
{
    int i, j, k = 0, a = 4;

    pt[k++] = 2;
    pt[k++] = 3;
    for (i = 5; i <= n; i+=(a^=6)) {
	if (pf[i]) continue;
	pt[k++] = i;
	for (j = i+(i<<1); j <= n; j += i<<1)
	    pf[j] = 1;
    }
    nr = k;
}

static int is_prime (unsigned int n)
{
    int i;

    for (i = 0; pt[i]*pt[i] <= n+1; i++) {
	if ((n % pt[i]) == 0)
	    return 0;
    }
    return 1;
}

static int is_sum_prime (int k)
{
    unsigned int sum = 0;
    int i, j;

    if ((k & 1) == 0) {
	for (i = 0; i < k; i++)
	    sum += pt[i];
	if (is_prime(sum) && sum <= pt[nr-1]) {
	    printf("%4d %-10u: ", k, sum);
	    printf("%d - %d\n", pt[0], pt[k]);
	    return 1;
	}
	return 0;
    }

    for (i = 0; i < k; i++)
	sum += pt[nr-i-1];
	    
    i = nr-k-1;
    while (i >= 0) {
	if (is_prime(sum) && sum <= pt[nr-1]) {
	    printf("%4d %-10u: ", k, sum);
	    printf("%d - %d\n", pt[i+1], pt[i+k]);
	    return 1;
	}
	sum += pt[i];
	sum -= pt[i+k];
	i--;
    }

    return 0;
}

int main (int argc, char *argv[])
{
    int n, m, i;

    scanf("%d", &n);

    sieve_p(n);
    m = sqrt(pt[nr-1]);

    printf("%d %d %d\n", m, pt[nr-1], nr);
    for (i = m; i > 3; i--) {
	is_sum_prime(i);
    }

    return 0;
}
