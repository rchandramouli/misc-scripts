/* PAN digital */
#include <stdio.h>

#include "mytypes.h"

static int is_pandigital (u64 n)
{
    int p = 0;

    while (n) {
        p |= (1 << (n % 10));
        n /= 10;
    }
    return (p == 1023 || p == 1022);
}

#define MAX_N  (9*8*7*6*5*4*3*2)

static int p_stats[10000];
static int pand_stats[MAX_N+1];

int pandigital_primes (void)
{
    u32 i;
    u64 p, q, u, s = 0;

    printf("Is 123456789 pandigital? %s\n",
           (is_pandigital(123456789ULL)) ? "yes": "no");
    fflush(stdout);

    for (p = 1000; p < 10000; p++) {
        for (q = 10000; q < 100000; q++) {
            u = p*q;
            if (is_pandigital(u)) {
                p_stats[p]++;
                pand_stats[u]++;
            }
        }
    }
    for (i = s = 0; i < MAX_N; i++) {
        if (pand_stats[i])
            s++;
    }
    printf("%I64u products are pandigital (%I64u primes)!\n",
           s, MAX_N-s);

    return 0;
}

#define PAN9_N (458976321ULL)

int pandigital_pdt (void)
{
    u64 p, n;

    for (n = 1; n < 1000; n++) {
        p = n*PAN9_N;
        if (is_pandigital(p))
            printf("%I64u * %3I64u = %I64u\n", PAN9_N, n, p);
    }
    return 0;
}

int main (int argc, char *argv[])
{
    //pandigital_primes();
    pandigital_pdt();

    return 0;
}
