#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define D_CONST   (12345)

int main (int argc, char *argv[])
{
    u64 r, v, k;
    long double x;

    r = 1;
    while (1) {
        x = (long double)D_CONST;
        x *= (logl((long double)r+1) / logl((long double)2.0));

        if (x < (long double)r)
            break;
        r++;
    }

    v = (logl((long double)r+1) / logl((long double)2.0));
    k = (1 << v);
    while (k < r) {
        if (v*12345ULL < k)
            break;
        k++;
    }

    printf("P(%I64u) = %I64u / %I64u\n", k*(k+1), v, k);

    return 0;
}
