#include <stdio.h>

#include "mytypes.h"

static u64 dsum (u64 N)
{
    int S = 0;

    while (N) {
        S += N % 10;
        N /= 10;
    }
    return S;
}

static u64 dpwrsum[100];

int main (int argc, char *argv[])
{
    int ds, p, c, i, j;
    u64 P;

    c = 0;
    for (ds = 6; ds < 150; ds++) {
        P = ds;
        i = 1;

        while (P < 100000000000000000ULL) {
            P *= ds;
            i++;
            if (dsum(P) == ds) {
                dpwrsum[c] = P;
                c++;
                printf("[%3d] %d^%d:  %I64u\n", c, ds, i, P);
            }
        }
    }

    for (i = 0; i < c; i++) {
        for (j = i+1; j < c; j++) {
            if (dpwrsum[i] > dpwrsum[j]) {
                P = dpwrsum[i];
                dpwrsum[i] = dpwrsum[j];
                dpwrsum[j] = P;
            }
        }
    }

    printf("\n");
    for (i = 0; dpwrsum[i]; i++) {
        printf("[%3d] %I64u\n", i+1, dpwrsum[i]);
    }
    printf("\n");
    return 0;
}
