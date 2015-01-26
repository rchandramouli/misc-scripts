/*
 * Area with triangles (a, a, a+1), (a, a, a-1)
 *
 * 4A = (a+1)SQRT[(3a+1)(a-1)]
 *         (or)
 * 4A = (a-1)SQRT[(3a-1)(a+1)]
 */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_PERI  (1000000000ULL)

int main (int argc, char *argv[])
{
    u64 a, sq, v, n;
    u64 S = 0, A;

    timeit_timer_start();

    for (n = 0, a = 2; a <= ((MAX_PERI-1)/3); a++) {
        /* Check parity */
        v = (3*a+1)*(a-1);
        A = ((a+1)*v) & 3;

        if (A) {
            continue;
        }

        /* Check (a,a,a+1) */
        sq = sqrtl(v);
        if (sq*sq == v) {
            S += 3*a+1;
            n++;
            printf("(%I64u, %I64u, %I64u), Area = %I64u\n",
                   a, a, a+1, ((a+1)*sq)/4);
        }

        /* Check (a,a,a-1) */
        v += 4*a;
        sq = sqrtl(v);
        if (sq*sq == v) {
            S += 3*a-1;
            n++;
            printf("(%I64u, %I64u, %I64u), Area = %I64u\n",
                   a, a, a-1, ((a-1)*sq)/4);
        }
    }
    timeit_timer_peek_and_print();

    printf("Number of triangles = %I64u\n"
           "Sum of perimeters = %I64u\n", n, S);

    return 0;
}
