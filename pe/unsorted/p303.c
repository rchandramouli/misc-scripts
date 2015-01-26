/* TBD */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N  (10000)

struct nincr_s {
    int s;
    int n;
    int delta[6];
};

/* Number ends with 'd' starting with '2' as multiplier */
static struct nincr_s incr[] = {
    /* 0 */ {0, 0},
    /* 1 */ {0, 3, {8, 1, 1}},
    /* 2 */ {3, 4, {1, 4, 1, 4}},
    /* 3 */ {2, 3, {3, 3, 4}},
    /* 4 */ {1, 4, {2, 3, 2, 3}},
    /* 5 */ {0, 5, {2, 2, 2, 2, 2}},
    /* 6 */ {0, 4, {3, 2, 3, 2}},
    /* 7 */ {1, 3, {3, 4, 3}},
    /* 8 */ {2, 4, {1, 4, 1, 4}},
    /* 9 */ {6, 3, {1, 1, 8}},
};

static int is_num012 (u64 N)
{
    while (N) {
        if ((N % 10) > 2)
            return 0;
        N /= 10;
    }
    return 1;
}

static u64 strip_10n (u64 N)
{
    while (!(N % 10))
        N /= 10;
    return N;
}

static u64 get_num012_multiple (u64 N, u64 f)
{
    u64 k, m, p;
    int d, t;

    if (is_num012(N))
        return 1;

    if ((N % 10) == 0) {
        u64 z = 1;

        while (!(N % 10)) {
            N /= 10;
            z *= 10;
        }
        return get_num012_multiple(N, N+1);
    }
    d = N % 10;
    m = f + incr[d].s;
    t = 0;

    do {
        k = m*N;
        m += incr[d].delta[t];
        t = (t + 1) % incr[d].n;
    } while (!is_num012(k));

    return (k/N);
}

int main (int argc, char *argv[])
{
    u64 n, v, s, m, k;

    timeit_timer_start();

    for (s = 0, n = 1; n <= MAX_N; n++) {
        m = n;
        if ((n % 10) == 0)
            m = strip_10n(n);
        /* Skips n's that are multiple of 99, 999 or 9999 */
        if ((m % 999) == 0 || (m % 99) == 0)
            continue;
        v = get_num012_multiple(m, 2);
        //printf("N = %I64d, PDT = %I64d\n", n, v);
        fflush(stdout);
        s += v;
    }
    printf("Sum = %I64d\n", s);

    timeit_timer_peek_and_print();

    for (n = 99, k = 1; k*n <= MAX_N; k++) {
        m = n*k;
        if ((m % 10) == 0)
            m = strip_10n(m);
        v = get_num012_multiple(m, 2);
        printf("N = %I64d, PDT = %I64d\n", n*k, v);
        fflush(stdout);
        s += v;
    }

    timeit_timer_peek_and_print();

    return 0;
}
