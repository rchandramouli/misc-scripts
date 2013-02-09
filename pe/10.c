#include <stdio.h>

#define MAX_P   2000000

static char pflag[MAX_P];
static int ptable[MAX_P], nr_primes;

static void p_sieve (void)
{
    int i, j, k;

    for (i = 2, k = 0; i < MAX_P; i++) {
	if (pflag[i]) continue;

	ptable[k] = i;
	k++;

	for (j = i+i; j < MAX_P; j+=i)
	    pflag[j] = 1;
    }
    nr_primes = k;
}

int main (int argc, char *argv[])
{
    int i;
    unsigned long long int s;

    p_sieve();

    for (i = s = 0; i < nr_primes; i++) {
	s += ptable[i];
    }

    for (i = 0; i < 100; i++) {
	printf("%d ", ptable[i]);
    }
    printf("\n\n");
    for (i = 0; i < 100; i++) {
	printf("%d ", ptable[nr_primes-i-1]);
    }
    printf("\n\n");
	    

    printf("%d %Lu\n", nr_primes, s);

    return 0;
}
