#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N  (10001)

static int get_cf_sqrtn_period (int n)
{
    int X[10001];
    int k, r, a1, b1, a2, b2, x;

    X[0] = b1 = sqrt(n);
    a1 = 1;
    k = 1;

    while (1) {
        a2 = (n - (b1*b1)) / a1;
        if (a2 == 1) {
            b2 = X[0];
            x = b1 + b2;
        } else {
            r = a2 - (b1 % a2);
            b2 = r;
            while (b2*b2 < n) {
                b2 += a2;
            }
            b2 -= a2;
            x = (b1 + b2) / a2;
        }

        X[k++] = x;
        a1 = a2;
        b1 = b2;

        if (x == 2*X[0])
            break;
    }

#ifdef DEBUG
    if (n < 100 || n > 9900) {
        printf("N = %5d --- [%d; (%d", n, X[0], X[1]);
        for (r = 2; r < k; r++)
            printf(", %d", X[r]);
        printf(")]\n");
    }
#endif

    return k;
}

int main (int argc, char *argv[])
{
    int n, c, s;

    for (n = 1, s = 0; n < MAX_N; n++) {
        c = sqrt(n);
        if (c*c == n)
            continue;
        if (!(get_cf_sqrtn_period(n) & 0x1))
            s++;
    }
    printf("Number of odd period square-roots (< %d) = %d\n", MAX_N, s);

    return 0;
}
