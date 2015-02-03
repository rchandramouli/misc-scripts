#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MOD_BIGNUM       (100000000ULL)
#define MAX_DIGIT_BLOCK  (16)
#include "bignum.h"

#define MAX_N   (1001)

static int get_next_coeff (int n, int *a1, int *b1)
{
    int x, a2, b2;

    a2 = (n - (*b1 * *b1)) / *a1;
    if (a2 == 1) {
        b2 = sqrt(n);
        x = b2 + *b1;
    } else {
        b2 = a2 - (*b1 % a2);
        while (b2*b2 < n) {
            b2 += a2;
        }
        b2 -= a2;
        x = (b2 + *b1) / a2;
    }

    *a1 = a2;
    *b1 = b2;

    return x;
}

static int check_xy (u64 n, struct BigNum *x, struct BigNum *y)
{
    struct BigNum bXX, bYY;

    BigInit(&bXX, 0);
    BigInit(&bYY, 0);

    BigMultiply(&bXX, x, x);
    BigMultiply(&bYY, y, y);
    BigMultiplyConst(&bYY, &bYY, n);
    BigAddConst(&bYY, &bYY, 1);

    if (BigCompare(&bXX, &bYY) == 0) {
#ifdef DEBUG
        printf("============= N = %I64u ==============\n", n);
        printf("\tX = "); BigPrint(x, x->n);
        printf("\tY = "); BigPrint(y, y->n);
        printf("\n");
        printf("\tX*X =   ");
        BigPrint(&bXX, bXX.n);
        printf("\tN*Y*Y = ");
        BigPrint(&bYY, bYY.n);
        printf("\n");
        fflush(stdout);
#endif
        return 1;
    }
    return 0;
}

int main (int argc, char *argv[])
{
    int n, x, a1, b1, t, mn;
    struct BigNum p[3], q[3], mx, my;

    BigInit(&mx, 0);
    BigInit(&my, 0);

    for (n = 2; n < MAX_N; n++) {
        a1 = 1;
        x = b1 = sqrt(n);

        if (x*x == n)
            continue;

        BigInit(&p[0], 1);
        BigInit(&q[0], 0);
        BigInit(&p[1], x);
        BigInit(&q[1], 1);

        if (check_xy(n, &p[1], &q[1])) {
            printf("N = %d, x = %I64u, y = %I64u\n", n, p[1], q[1]);
            fflush(stdout);
            continue;
        }
        do {
            x = get_next_coeff(n, &a1, &b1);

            BigMultiplyConst(&p[2], &p[1], x);
            BigAdd(&p[2], &p[2], &p[0]);

            BigMultiplyConst(&q[2], &q[1], x);
            BigAdd(&q[2], &q[2], &q[0]);

            if (check_xy(n, &p[2], &q[2])) {
                break;
            }

            BigAssign(&p[0], &p[1]);
            BigAssign(&p[1], &p[2]);
            BigAssign(&q[0], &q[1]);
            BigAssign(&q[1], &q[2]);
        } while (1);

        if (BigCompare(&mx, &p[2]) < 0) {
            BigAssign(&mx, &p[2]);
            BigAssign(&my, &q[2]);
            mn = n;
        }
    }
    printf("\n===== Maximum at N = %d\n", mn);
    printf("X: "); BigPrint(&mx, mx.n);
    printf("Y: "); BigPrint(&my, my.n);

    return 0;
}
