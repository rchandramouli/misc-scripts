/* TODO - takes hell-a-lota time, remember the 1min rule. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE (20000000ULL)
#include "prime.h"

#ifdef DEBUG
#define dprintf printf
#else
#define dprintf(...)
#endif

struct factors {
    u64 p[8];
    u32 n;
};

struct factors *f;

static void factorize_all_numbers (void)
{
    u64 i, j, p;

    f = calloc(sizeof(struct factors), MAX_PTABLE_SIZE+1);
    if (!f) {
        printf("%s: memory allocation failed!\n", __func__);
        return;
    }

    for (i = 0; i < nr_primes; i++) {
        p = ptable[i];

        /* Sieve all numbers that are multiple of p */
        for (j = p; j <= MAX_PTABLE_SIZE; j += p) {
            f[j].p[f[j].n++] = p;
        }

        /* Sieve all numbers that are power's of p */
        p *= ptable[i];
        while (p <= MAX_PTABLE_SIZE) {
            for (j = p; j <= MAX_PTABLE_SIZE; j += p)
                f[j].p[f[j].n-1] *= ptable[i];
            p *= ptable[i];
        }
    }
}

static u64 get_max_modular_inverse (u64 num)
{
    u64 x, mp, np, v, v1, v2;
    int c, i, si, found;

    if (f[num].n == 1)
        return 1;

    /* Find the maximal product */
    mp = f[num].p[0];
    si = 0;
    for (i = 1; i < f[num].n; i++) {
        if (mp < f[num].p[i]) {
            mp = f[num].p[i];
            si = i;
        }
    }
    x = num / mp - 1;

    dprintf("N = %I64d (n = %d)\n", num, f[num].n);

    for (v = 1; x >= 0; x--) {
        found = 0;
        v = x*mp;

        v1 = v + 1;
        v2 = v + mp - 1;

        dprintf("\tv = %I64d, v1 = %I64d, v2 = %I64d\n",
                v, v1, v2);

        for (c = 1, i = 0; i < f[num].n; i++) {
            if (i == si) continue;
            np = f[num].p[i];

            if ((((v1 - 1) % np) == 0) ||
                (((v1 - np + 1) % np) == 0)) {
                c++;
            }
            dprintf("\t\tv1-1 = %I64d, v1-np+1 = %I64d, np = %I64d, c = %d\n",
                    v1-1, v1-np+1, np, c);
        }

        if (c == f[num].n) {
            dprintf("\tv = %I64d, v1 = %I64d\n", v, v1);
            v = v1;
            found = 1;
        }
        if (v2 == num-1) {
            if (found)
                break;
            continue;
        }
        for (c = 1, i = 0; i < f[num].n; i++) {
            if (i == si) continue;
            np = f[num].p[i];

            if ((((v2 - 1) % np) == 0) ||
                (((v2 - np + 1) % np) == 0)) {
                c++;
            }
            dprintf("\t\tv2-1 = %I64d, v2-np+1 = %I64d, np = %I64d, c = %d\n",
                    v2-1, v2-np+1, np, c);
        }
        if (c == f[num].n) {
            dprintf("\tv = %I64d, v2 = %I64d\n", v, v2);
            v = v2;
            break;
        }
        if (x == 0 || found)
            break;
    }
    return (v == 0) ? 1: v;
}

#ifdef DEBUG
static void dump_factors (u32 num)
{
    u32 j;

    for (j = 0; j < f[num].n; j++) {
        printf("%I64d ", f[num].p[j]);
    }
}
#else
#define dump_factors(...)
#endif

int main (int argc, char *argv[])
{
    u64 S, i, j, v;

    timeit_timer_start();
    generate_primes();
    printf("Generated %I64d primes\n", nr_primes);
    timeit_timer_peek_and_print();

    printf("Factorizing all numbers\n");
    factorize_all_numbers();
    timeit_timer_peek_and_print();

    /* For all primes, 1^1 = 1 (mod p), skip 2 since n >= 3 */
    S = nr_primes - 1;

    for (i = 1; i < nr_primes-1; i++) {
        if ((i % 30000) == 0) {
            timeit_timer_peek_and_print();
            printf("DONE for %I64d primes\n", i);
            fflush(stdout);
        }
        for (j = ptable[i]+1; j <= ptable[i+1]-1; j++) {
            v = get_max_modular_inverse(j);
            dprintf("%I64d: ( ", j);
            dump_factors(j);
            dprintf(") %I64d\n", v);
            S += v;
        }
    }
    for (j = ptable[nr_primes-1]+1; j <= MAX_PTABLE_SIZE; j++) {
        v = get_max_modular_inverse(j);
        dprintf("%I64d: ( ", j);
        dump_factors(j);
        dprintf(") %I64d\n", v);
        S += v;
    }
    timeit_timer_peek_and_print();

    printf("Modular inverse sum = %I64d\n", S);

    return 0;
}
