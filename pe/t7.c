#include <stdio.h>
#include <math.h>

#include "mytypes.h"

int main (int argc, char *argv[])
{
    u64 n, v, q;

    for (n = 1; n < 100000000; n++) {
        v = 5*n*n + 2*n + 1;
        q = sqrtl(v);
        if (q*q == v) {
            printf("N = %I64u\n", n);
            fflush(stdout);
        }
    }
    return 0;
}
