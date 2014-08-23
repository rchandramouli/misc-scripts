#ifndef PRIME_H
#define PRIME_H

#include <math.h>

#ifndef MAX_PRIMES
#define MAX_PRIMES    1000
#endif

static char p_flag[MAX_PRIMES];
static int p_table[MAX_PRIMES], nr_primes;

static void p_sieve (int max_n)
{
    int i, j, k;

    for (i = 2, k = 0; i < max_n; i++) {
        if (p_flag[i]) continue;

        p_table[k] = i;
        k++;

        for (j = i+i; j < max_n; j += i)
            p_flag[j] = 1;
    }
    nr_primes = k;
}

static int is_prime (int n)
{
    int l, i;

    if (n < 0)
        n = -n;

    if (n == 0 || n == 1)
        return 0;

    if (n < MAX_PRIMES) {
        return !p_flag[n];
    } else {
        l = sqrt(n);
        for (i = 0; i < nr_primes; i++) {
            if ((n % p_table[i]) == 0)
                return 0;
        }
        for (i = p_table[nr_primes-1] + 2; i <= l; i+= 2) {
            if ((n % i) == 0)
                return 0;
        }
    }
    return 1;
}
#endif /* PRIME_H */
