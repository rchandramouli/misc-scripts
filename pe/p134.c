/*
 * Find minimal N such that (p1 < p2),
 *
 *      (N*10^k + p1) = 0 (mod p2)
 *
 * Note that the numbers can overlap!!
 */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE   (1100000UL)
#include "prime.h"

#define MAX_N    (1000000UL)
//#define MAX_N    (100UL)

static u64 pow10[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000
};


/* NOTE: Extended Euclidean algorithm */
static u64 get_inverse (u64 n, u64 p)
{
    s64 t0 = 0, t1 = 1, tt;
    s64 r0 = p, r1 = n, tr;
    s64 q;

    while (r1) {
        q = r0 / r1;

        tt = t1;
        t1 = t0 - q*t1;
        t0 = tt;

        tr = r1;
        r1 = r0 - q*r1;
        r0 = tr;
    }
    if (t0 < 0)
        t0 += p;
    return t0;
}

static u64 get_inverse_exp (u64 p, u64 r)
{
    u64 pk, k, t, v, mod[10];

    pk = 1;
    k = 0;
    do {
        pk = (pk * 10) % p;
        mod[k % 10] = pk;
        k++;
    } while (pk != 1);

    v = k*(r/k+1) - r;

    return mod[v % 10];
}

static u64 get_minimal_value (u64 p1, u64 p2, u64 k)
{
    u64 v, r = 1, t = k;

    while (t--)
        r = (r * 10) % p2;

    //v = (get_inverse_exp(p2, k) * (p2 - p1)) % p2;
    v = (get_inverse(r, p2) * (p2 - p1)) % p2;
    while (k--)
        v *= 10;
    v += p1;

    return v;
}

int main (int argc, char *argv[])
{
    u64 s = 0, v;
    u64 p1, p2, i, k;
    int idx = 0;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    for (i = 3; i < nr_primes; i++) {
        p1 = ptable[i-1];
        p2 = ptable[i];
        if (p1 >= MAX_N) {
            printf("P1 = %I64u, > %I64u!\n", p1, MAX_N);
            break;
        }

        if (p1 > pow10[idx+1]) {
            idx++;
        }

        k = idx+1;
        v = get_minimal_value(p1, p2, k);
        if ((MAX_N <= 100) || (i % 10000) == 0) {
            printf("P1 = %lu, P2 = %lu, v = %I64u (v/P2 = %I64u, mod: %I64u)\n",
                   p1, p2, v, v/p2, (v % p2));
            fflush(stdout);
        }
        s += v;
    }

    printf("\nSum = %I64u\n", s);
    timeit_timer_peek_and_print();

    return 0;
}
