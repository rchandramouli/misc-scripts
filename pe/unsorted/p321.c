/*
 * Solve for: n(n+2) = t(t+1)/2
 *
 * t^2 + t - (2n^2 + 4n) = 0
 *
 * The above gives the Pell's equation: T^2 - 2N^2 = -7,
 *          N = 2(n+1), T = 2t+1
 */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "prime.h"

#define MAX_N   1000000000ULL

int main (int argc, char *argv[])
{
    u64 t, v, s;
    char f;

    for (f = 0, t = 1; t <= MAX_N; t++) {
        if (t & 1) {
            v = ((t+1)/2)*t + 1;
        } else {
            v = (t/2)*(t+1) + 1;
        }
        s = sqrtl(v);
        if (s*s == v) {
            printf("T = %I64d, N = %I64d\n", t, s-1);
            if (f) {
                t += t + t/2;
            } else {
                t += t;
            }
            f ^= 1;
        }
    }
    return 0;
}
