/* Convergents of e:
 *
 * e = 2 + 1/1+ 1/2+ 1/1+ 1/1+ 1/4+ 1/1+ ... 1/1+ 1/2k+ 1/1+ ...
 *
 * p(k+1) = b(k+1)*p(k) + p(k-1)
 * q(k+1) = b(k+1)*q(k) + q(k-1)
 *
 * p(0) = 2, p(1) = 3
 * q(0) = 1, q(1) = 1
 *
 * b(k) = 2(k+1)/3, k = 2 (mod 3),
 *      = 1, otherwise
 */
#include <stdio.h>

#include "mytypes.h"
#include "bignum.h"

int main (int argc, char *argv[])
{
    struct BigNum p0, p1, pn, t;
    int i, b;

    BigInit(&p0, 2);
    BigInit(&p1, 3);

    for (i = 2; i < 100; i++) {
        if ((i % 3) == 2) {
            b = 2*((i+1)/3);
            BigMultiplyByConst(&t, &p1, b);
            BigAdd(&pn, &t, &p0);
        } else {
            b = 1;
            BigAdd(&pn, &p1, &p0);
        }
        BigPrint(&pn);
        BigAssign(&p0, &p1);
        BigAssign(&p1, &pn);
    }
    printf("\nDigit Sum = %d\n", BigDigitSum(&pn));

    return 0;
}
