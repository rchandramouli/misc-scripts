/* FIXME: Does not work, revisit!!! */
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

typedef unsigned long long int u64;
typedef unsigned long int u32;

#define bign_t   u64
#define bigfmt   "%llu"

static bign_t ptable[10000], nr_p;

static int is_prime (bign_t n)
{
	bign_t sq = sqrt((double)n);
	bign_t i;

	for (i = 0; i < nr_p; i++) {
		if (ptable[i] > sq)
			return 1;
		if ((n % ptable[i]) == 0)
			return 0;
	}
	return 1;
}

static void generate_primes (void)
{
	bign_t i, j;

	nr_p = 2;
	ptable[0] = 2;
	ptable[1] = 3;

	for (i = 5; i < 10000; i += 2) {
		for (j = 0; j < nr_p; j++)
			if ((i % ptable[j]) == 0)
				break;
		if (j == nr_p) {
			ptable[j] = i;
			nr_p++;
		}
	}
}

static bign_t ppdt[10], nr_pdt;
static bign_t get_plist (bign_t n)
{
	bign_t i;
	bign_t max = 1;

	nr_pdt = 0;
	for (i = 0; i < nr_p && n > ptable[i]; i++) {
		if (n % ptable[i])
			continue;
		ppdt[nr_pdt] = 1;
		while (!(n % ptable[i])) {
			n /= ptable[i];
			ppdt[nr_pdt] *= ptable[i];
		}
		if (max < ppdt[nr_pdt])
			max = ppdt[nr_pdt];
		nr_pdt++;
	}
	if (n > 1) {
		ppdt[nr_pdt] = n;
		if (max < ppdt[nr_pdt])
			max = ppdt[nr_pdt];
		nr_pdt++;
	}
	return max;
}

static bign_t get_a_f (bign_t n, bign_t f)
{
	bign_t m, t = 0;

	if (n == f)
		return 1;

	for (m = f; m < n-1; m += f)
		if (((m*(m+1)) % n) == 0) {
			if (m+1 > t)
				t = m+1;
		} else if (((m*(m-1)) % n) == 0) {
			if (m > t)
				t = m;
		}
	return t;
}

int main (int argc, char *argv[])
{
	bign_t n, f, s = 0;

	generate_primes();

	scanf(bigfmt, &n);
	for (; n > 1; n--) {
		f = get_plist(n);
		s += get_a_f(n, f);
	}
	printf(bigfmt"\n", s);

	return 0;
}
