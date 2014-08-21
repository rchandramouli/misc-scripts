#include <stdio.h>

#define FILL_SUBARRAY_BODY                              \
    do {                                                \
        if (!f) {                                       \
            if (arr[i][j] < 0) {                        \
                f1 = 0;                                 \
            } else {                                    \
                f1 = 1;                                 \
            }                                           \
            f = 1;                                      \
            subarr[l = 0] = 0;                          \
        }                                               \
        if (!f1 && arr[i][j] >= 0) {                    \
            f1 = 1;                                     \
            subarr[++l] = 0;                            \
        } else if (f1 && arr[i][j] < 0) {               \
            f1 = 0;                                     \
            subarr[++l] = 0;                            \
        }                                               \
        subarr[l] += arr[i][j];                         \
    } while (0)

static long long int arr[2000][2000];
static long long int subarr[2000];

static long long int get_max_subarray (int len, long long int mx)
{
    long long int m = subarr[0], ps, s;
    int i, j, k, f;

    for (i = f = s = ps = 0; i < len; i++) {
        if (!f && subarr[i] <= 0)
            continue;
        f = 1;
        
    return (m > mx ? m: mx);
}

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
    int i, j, l, k, f, f1, M;
    long long int s, ps, mh, mv, md, mad;

    M = 2000;
    generate_lagged_fib_array(M);

    /* Sum all horizontal lines */
    mh = 0;
    for (i = 0; i < M; i++) {
        f = 0;
        for (j = 0; j < M; j++) {
            FILL_SUBARRAY_BODY;
        }
        mh = get_max_subarray(l, mh);
    }
    printf("Horizontal maximum: %lld\n", mh);

    /* Vertical */
    mv = 0;
    for (j = 0; j < M; j++) {
        f = 0;
        for (i = 0; i < M; i++) {
            FILL_SUBARRAY_BODY;
        }
        mv = get_max_subarray(l, mv);
    }
    printf("Vertical maximum: %lld\n", mv);

    /* Diagonals */
    md = 0;
    for (k = 0; k < M; k++) {
        f = 0;
        for (i = k, j = 0; i < M; i++, j++) {
            FILL_SUBARRAY_BODY;
        }
        md = get_max_subarray(l, md);
    }
    for (k = 0; k < M-1; k++) {
        f = 0;
        for (j = k+1, i = 0; j < M; i++, j++) {
            FILL_SUBARRAY_BODY;
        }
        md = get_max_subarray(l, md);
    }
    printf("Diagonal maximum: %lld\n", md);

    /* Anti-Diagonals */
    mad = 0;
    for (k = M-1; k >= 0; k--) {
        f = 0;
        for (i = 0, j = k; j >= 0; i++, j--) {
            FILL_SUBARRAY_BODY;
        }
        mad = get_max_subarray(l, mad);
    }
    for (k = 1; k < M; k++) {
        s = ps = f = 0;
        for (i = k, j = M-1; i < M; i++, j--) {
            FILL_SUBARRAY_BODY;
        }
        mad = get_max_subarray(l, mad);
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
