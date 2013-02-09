#include <stdio.h>

#define MAX_P   200000

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

static unsigned long long int get_divisor_count (unsigned long long int n)
{
	unsigned long long int div = 1;
	int count, idx;

	idx = count = 0;
	while (idx < nr_primes) {
		if ((n % ptable[idx]) == 0) {
			n /= ptable[idx];
			count++;
		} else if (n == 1) {
			div *= (1 + count);
			break;
		} else {
			div *= (1 + count);
			count = 0;
			idx++;
		}
	}
	return div;
}

int main (int argc, char *argv[])
{
	unsigned long long int t = 0, i, nd, v;
	p_sieve();

	scanf("%Lu", &v);
	for (i = 1; i < 100000; i++) {
		t += i;

		if ((nd = get_divisor_count(t)) > v) {
			printf("%Lu: %Lu\n", t, nd);
			break;
		}
	}
	return 0;
}
