#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

static int gcd (int a, int b)
{
    if (a > b)
        return gcd(b, a);
    if (a == 0)
        return b;

    return gcd(b%a, a);
}

#define MAX_N   (12000)
//#define MAX_N   (8)

int main (int argc, char *argv[])
{
    int p, q;
    u32 S = 0;

    timeit_timer_start();

    for (q = 5; q <= MAX_N; q++) {
        for (p = 1+q/3; p <= q/2; p++) {
            if (gcd(p, q) == 1) {
                S++;
            }
        }
    }
    timeit_timer_peek_and_print();

    printf("Total fractions = %lu\n", S);

    return 0;
}
