/*
 * Set (a1,b2,c3,d4,a4,b3,c1,c2,d1,d2) as free variables with one
 * constraint that the diagonal should sum upto same number. This
 * gives the following set of equations:
 *
 * a2 = K - b2 - c2 - d2
 * b1 = K - a1 - d1 - c1
 * c4 = K - c2 - c3 - c1
 * d3 = K - d1 - d4 - d2
 * a3 = K - a1 - a2 - a4
 * d3 = K - b3 - c3 - a3
 * b4 = K - b2 - b3 - b1
 * c4 = K - a4 - d4 - b4
 */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define DLOOP(v)           for (v = 0; v < 10; v++)
#define CHECK_IF_CONT(v)   if ((v) < 0 || (v) > 9) continue

int main (int argc, char *argv[])
{
    int a1, a2, a3, a4;
    int b1, b2, b3, b4;
    int c1, c2, c3, c4;
    int d1, d2, d3, d4;
    int K;
    u64 C = 0, arr[37];

    for (K = 0; K <= 36; K++)
        arr[K] = 0;

    /* (c1, c4), (d2, d3) are independent as well, so, cut d2 & c1. */
    DLOOP(a1) DLOOP(a4) DLOOP(b2) DLOOP(b3) DLOOP(c2)
        DLOOP(c3) DLOOP(d1) DLOOP(d4) DLOOP(d2) DLOOP(c1) {

        K = a1+b2+c3+d4;
        if (K != a4+b3+c2+d1)
            continue;

        if (a1+d1+a4+d4 != b2+b3+c2+c3)
            continue;

        a2 = K-b2-c2-d2;
        CHECK_IF_CONT(a2);

        a3 = K-a1-a2-a4;
        CHECK_IF_CONT(a3);

        b1 = K-a1-d1-c1;
        CHECK_IF_CONT(b1);

        b4 = K-b1-b2-b3;
        CHECK_IF_CONT(b4);

        c4 = K-c2-c3-c1;
        CHECK_IF_CONT(c4);

        d3 = K-d1-d2-d4;
        CHECK_IF_CONT(d3);

        C++;
        arr[K]++;
        if (K == 1 || K == 2 || K == 35 || K == 36) {
            printf("K = %2d (%d %d %d %d, %d %d %d %d, "
                   "%d %d %d %d, %d %d %d %d)\n", K,
                   a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4,
                   d1, d2, d3, d4);
            fflush(stdout);
        }
    }
    printf("Number of possible squares = %I64u\n", C);
    printf("\n");
    for (K = 0; K <= 36; K++) {
        printf("K = %d, Squares = %I64u\n", K, arr[K]);
    }
    printf("\n");

    return 0;
}
