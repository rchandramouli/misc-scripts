/*
 * Maximize (N/k)^k for a given N and sum the following function:
 * D(N) = N if 1/k is terminating, else -N.
 */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

int main (int argc, char *argv[])
{
    u64 N, k;
    s64 S;

    S = 0;

    for (N = 5; N <= 100; N++) {
        k = llround((double)N / M_E);

        if (((k % 2) == 0) ||
            ((k % 5) == 0)) {
            S += -N;
        } else {
            S += N;
        }
    }

    printf("Sum = %I64d\n", S);

    return 0;
}
