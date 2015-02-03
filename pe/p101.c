/*
 * Note that, to find F(n+1), given values of F(1)..F(n) for a
 * degree-n polynomial, it is not required to interpolate and find the
 * value of coefficients of the polynomial!
 */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_COEFF   (10)

static s64 poly[MAX_COEFF+1];
static long int pdt[MAX_COEFF+1];
static s64 pascal_tbl[MAX_COEFF+1][MAX_COEFF+1];

static s64 get_poly_sum (s64 d)
{
    s64 S;
    int sign = 1, i;

    for (S = i = 0; i <= d; i++) {
        S += pascal_tbl[d][i] * poly[d-i] * sign;
        sign = -sign;
    }
    return S;
}

/* 1-n+n^2-n^3+n^4-n^5+n^6-n^7+n^8-n^9+n^10 */
static s64 get_poly10 (s64 N)
{
    s64 N2, N4, N8;

    N2 = N * N;
    N4 = N2 * N2;
    N8 = N4 * N4;

    return (1 + (N2 - N)*((1 + N2)*(1 + N4) + N8));
}

static u64 get_poly3 (s64 N) { return (N*N*N); }
static u64 get_poly4 (s64 N) { return (N*N*N*N); }

static void fill_pascal_table (int n)
{
    int i, j;

    for (i = 0; i <= n; i++) {
        pascal_tbl[i][0] = pascal_tbl[i][i] = 1;
    }
    for (i = 2; i <= n; i++) {
        for (j = 1; j <= i-1; j++) {
            pascal_tbl[i][j] = pascal_tbl[i-1][j] + pascal_tbl[i-1][j-1];
        }
    }

    /* Sum row-wise */
    for (i = 1; i <= n; i++) {
        for (j = 0; j <= i-1; j++) {
            pascal_tbl[i][j] += pascal_tbl[i-1][j];
        }
    }
}

static void dump_pascal_table (int n)
{
    int i, j;

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= i; j++) {
            printf("%I64d ", pascal_tbl[i][j]);
        }
        printf("\n");
    }
}

static void dump_poly_table (int n)
{
    int i;

    for (i = 0; i <= n; i++) {
        printf("%I64d ", poly[i]);
    }
    printf("\n");
}

int main (int argc, char *argv[])
{
    int i;
    s64 S, u0, u1;

    /* Fill polynomial points */
    for (i = 0; i <= MAX_COEFF+1; i++)
        poly[i] = get_poly10(i+1);

    fill_pascal_table(MAX_COEFF);
    dump_pascal_table(MAX_COEFF);
    dump_poly_table(MAX_COEFF+1);

    for (S = i = 0; i <= MAX_COEFF; i++) {
        printf("BOP of deg(%d) = %I64d\n", i, get_poly_sum(i));
        if (i == MAX_COEFF)
            break;
        S += get_poly_sum(i);
    }

    printf("\nSum of BOP = %I64d\n", S);

    return 0;
}
