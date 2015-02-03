#include <stdio.h>

#include "mytypes.h"

#define MOD_N10   (10000000000ULL)
#define MOD_N5    (100000ULL)

static u64 get_square_mod (u64 N)
{
    u64 a, b, p;

    if (!(N / (MOD_N10/10)))
        return ((N * N) % MOD_N10);

    /* Now, this product overflows, handle appropriately. */
    a = N / MOD_N5;
    b = N % MOD_N5;

    p = (b*b) % MOD_N10;
    p = (p + a*b*MOD_N5) % MOD_N10;
    p = (p + a*b*MOD_N5) % MOD_N10;

    return p;
}

static u64 compute_mod_power (u64 N)
{
    u64 k, p = 1;

    k = 1;
    while (k <= N)
        k <<= 1;
    if (k > N)
        k >>= 1;

    while (k) {
        printf("P = %I64u (k = %I64d)\n", p, k);
        p = get_square_mod(p);
        if (k & N)
            p = (p * 2) % MOD_N10;
        k >>= 1;
    }
    return p;
}

#define P2   (7830457ULL)
#define A    (28433ULL)
#define B    (1ULL)

int main (int argc, char *argv[])
{
    u64 p;

    p = compute_mod_power(P2);
    p = (p * A) % MOD_N10;
    p = (p + B) % MOD_N10;

    printf("%I64u * 2^%I64u + %I64u (mod %I64u) = %I64u\n",
           A, P2, B, MOD_N10, p);

    return 0;
}
