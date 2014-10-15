#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE (10000001ULL)
#include "prime.h"

#ifndef INPUT_N
#define INPUT_N    (100000000ULL)
#endif
#define MOD_N      (1000000009ULL)

static u64 N = INPUT_N;

static u64 get_prime_mod_pdt (u64 p)
{
    u64 pdt = 1, k, c = 0, v, t;

    k = p;
    do {
        v = N / k;
        c += v;
        k *= p;
    } while (v);

    c *= 2;
    t = c;

    /* Do repeated squaring */
    k = p;
    do {
        if (c & 1) {
            pdt = (pdt * k) % MOD_N;
        }
        c >>= 1;
        k = (k * k) % MOD_N;
    } while (c);
    pdt = (1 + pdt) % MOD_N;

    if (p == 2 || p == 3 || p == 5 || p == 7 || p == 11 ||
        (p > ptable[nr_primes-1] && p < ptable[nr_primes-1]+100000)) {
        printf("\t%s: %I64d ^ %I64d, pdt = %I64d\n",
               __func__, p, t, pdt);
    }
    return pdt;
}

int main (int argc, char *argv[])
{
    u64 r_pdt = 1, i;
    u32 pc = 0;

    timeit_timer_start();
    generate_primes();
    printf("Generated %I64d primes\n", nr_primes);
    timeit_timer_peek_and_print();

    for (i = 0; i < nr_primes && ptable[i] <= N; i++) {
        r_pdt = (r_pdt * get_prime_mod_pdt(ptable[i])) % MOD_N;
        pc++;
    }
    timeit_timer_peek_and_print();

    printf("Number of primes = %d, sum of squares of factors = %I64d\n",
           pc, r_pdt);

    if (ptable[nr_primes-1] < N) {
        for (i = ptable[nr_primes-1]+2; i <= N; i += 2) {
            if (is_prime(i)) {
                printf("%s: testing for prime %I64d\n", __func__, i);
                r_pdt = (r_pdt * get_prime_mod_pdt(i)) % MOD_N;
                pc++;
            }
        }
    } else {
        printf("%s: ptable[%I64d] = %I64d, N = %I64d\n", __func__, nr_primes-1,
               ptable[nr_primes-1], N);
    }
    timeit_timer_peek_and_print();

    printf("Number of primes = %d, sum of squares of factors = %I64d\n",
           pc, r_pdt);
    return 0;
}
