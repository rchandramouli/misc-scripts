#include <stdio.h>

#define MAX(a, b)   (((a) < (b)) ? (b): (a))

#define APPLY_KADANE_ALGORITHM                         \
    do {                                               \
        if (!f) {                                      \
            max_sf = max_end = arr[i][j];              \
            f = 1;                                     \
            continue;                                  \
        }                                              \
        max_end = MAX(arr[i][j], max_end + arr[i][j]); \
        max_sf = MAX(max_end, max_sf);                 \
    } while (0)

static long long int arr[2000][2000];

static void generate_lagged_fib_array (int n)
{
    long long int *p, k;

    p = &arr[0][0];

    for (k = 0; k < 55; k++) {
        p[k] = ((100003 - 200003*(k+1) + 300007*(k+1)*(k+1)*(k+1)) % 1000000) - 500000;
    }
    for (k = 55; k < 2000*2000; k++) {
        p[k] = ((p[k-24] + p[k-55] + 1000000) % 1000000) - 500000;
    }
    printf("p[10] = %lld, p[100] = %lld\n", p[9], p[99]);
}

int main (int argc, char *argv[])
{
    int i, j, k, f, M;
    long long int max_sf, max_end;
    long long int mh, mv, md, mad;

    M = 2000;
    generate_lagged_fib_array(M);

    /* Sum all horizontal lines */
    for (i = mh = 0; i < M; i++) {
        for (j = f = 0; j < M; j++) {
            APPLY_KADANE_ALGORITHM;
        }
        mh = MAX(mh, max_sf);
    }
    printf("Horizontal maximum: %lld\n", mh);

    /* Vertical */
    for (j = mv = 0; j < M; j++) {
        for (i = f = 0; i < M; i++) {
            APPLY_KADANE_ALGORITHM;
        }
        mv = MAX(mv, max_sf);
    }
    printf("Vertical maximum: %lld\n", mv);

    /* Diagonals */
    for (k = md = 0; k < M; k++) {
        for (i = k, j = f = 0; i < M; i++, j++) {
            APPLY_KADANE_ALGORITHM;
        }
        md = MAX(md, max_sf);
    }
    for (k = 0; k < M-1; k++) {
        for (j = k+1, i = f = 0; j < M; i++, j++) {
            APPLY_KADANE_ALGORITHM;
        }
        md = MAX(md, max_sf);
    }
    printf("Diagonal maximum: %lld\n", md);

    /* Anti-Diagonals */
    for (k = M-1, mad = 0; k >= 0; k--) {
        for (i = f = 0, j = k; j >= 0; i++, j--) {
            APPLY_KADANE_ALGORITHM;
        }
        mad = MAX(mad, max_sf);
    }
    for (k = 1; k < M; k++) {
        for (i = k, j = M-1, f = 0; i < M; i++, j--) {
            APPLY_KADANE_ALGORITHM;
        }
        mad = MAX(mad, max_sf);
    }
    printf("Anti-Diagonal maximum: %lld\n", mad);

    if (mh < mv)
        mh = mv;
    if (mh < md)
        mh = md;
    if (mh < mad)
        mh = mad;
    printf("\nMaximum sum = %lld\n", mh);

    return 0;
}
