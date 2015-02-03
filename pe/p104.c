#include <stdio.h>

#include "timeit.h"

#define MAX_DIGIT_BLOCK (5000)
#include "bignum.h"

#define MOD_N10   (1000000000ULL)

static inline int is_pandigital (u32 n)
{
    char f[10];

    f[0] = f[1] = f[2] = f[3] = f[4] =
        f[5] = f[6] = f[7] = f[8] = f[9] = 0;

    while (n) {
        f[n%10] = 1;
        n /= 10;
    }
    return (f[1] & f[2] & f[3] & f[4] & f[5] &
            f[6] & f[7] & f[8] & f[9]);
}

static int is_last_pandigital (struct BigNum *A)
{
    int n = A->d[0] % MOD_N10;

    return is_pandigital(n);
}

static int is_first_pandigital (struct BigNum *A)
{
    int i, d;
    u64 u, v;

    v = A->d[A->n-1];
    if (v*10 > MOD_N10) {
        while (v > MOD_N10)
            v /= 10;
        return is_pandigital(v);
    }

    d = 0;
    while (v < MOD_N10) {
        v *= 10;
        d++;
    }
    v /= 10;
    d--;

    u = A->d[A->n-2];
    for (i = 16; i != d; i--)
        u /= 10;
    v += u;

    return is_pandigital(v);
}

int main (int argc, char *argv[])
{
    struct BigNum F0, F1, F2;
    int i = 3;
    char fl, ff, ffl, f;

    BigInit(&F0, 1);
    BigInit(&F1, 1);

    fl = ff = ffl = 0;

    timeit_timer_start();
    while (1) {
        f = 0;
        BigAdd(&F2, &F0, &F1);
        BigAssign(&F0, &F1);
        BigAssign(&F1, &F2);

        if (is_last_pandigital(&F2)) {
            if (!fl) {
                printf("-LAST-  N = %5d   ", i);
                BigPrint(&F2, 6);
                fl = 1;
            }
            f++;
        }
        if (is_first_pandigital(&F2)) {
            if (!ff) {
                printf("-FIRST- N = %5d   ", i);
                BigPrint(&F2, 6);
                ff = 1;
            }
            f++;
        }
        if (f == 2) {
            printf("-FIRST/LAST- N = %8d    ", i);
            BigPrint(&F2, 6);
            break;
        }
        i++;
    }
    timeit_timer_peek_and_print();

    return 0;
}
