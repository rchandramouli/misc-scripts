/*
 * f(p) = number of bits set in prime p
 * f(m*n) = f(m) + f(n)
 *
 * f(0) = 0, f(1) = 0
 */
#include <stdio.h>

#define MAX_PRIMES 10000
#include "prime.h"

typedef unsigned long int u32;
typedef unsigned long long int u64;

static inline u32 count_ones (u32 n)
{
    u32 c = 0;

    while (n) {
        c++;
        n = n & (n-1);
    }
    return c;
}

static u32 get_composite_ones (u32 n)
{
    int i, c, s;

    s = 0;
    for (i = 0; i < nr_primes; i++) {
        c = 0;
        while ((n % p_table[i]) == 0)  {
            c++;
            n /= p_table[i];
        }
        if (c)
            s += c*count_ones(p_table[i]);
    }
    return s;
}

int main (int argc, char *argv[])
{
    int i, v;

    p_sieve(MAX_PRIMES);

    printf("%3d %3d ", 0, 0);
    for (i = 2; i < MAX_PRIMES; i++) {
        if (is_prime(i)) {
            v = count_ones(i);
        } else {
            v = get_composite_ones(i);
        }
        if ((i % 30) == 0)
            printf("\n");
        printf("%3d ", v);
    }
    printf("\n");

    return 0;
}
