#include <stdio.h>

/*
 * Maximize: x1 * x2^2 * x3^3 * ... * xm^m
 *
 * Under, x1 + x2 + ... + xm = m
 *
 * Using AM >= GM, MAX-VALUE = ((2/(m+1))^m(m+1)/2) * 2^2 * 3^3 * ... * m^m
 */
typedef unsigned long long int u64;

int main (int argc, char *argv[])
{
    u64 p, s;
    long double v;
    int i, j, m;


    for (s = 0, m = 2; m <= 15; m++) {
        v = 2.0 / (long double)(m+1);
        for (i = 2; i <= m; i++) {
            for (j = 1; j <= i; j++) {
                v *= (2.0 * (long double)i) / (long double)(m + 1);
            }
        }

        p = v;
        s += p;
        printf("M = %d, p = %I64d\n", m, p);
    }
    printf("\nSum = %I64d\n", s);

    return 0;
}

