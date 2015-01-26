/* TBD */
/*
 * F(0) = F(1) = 1, F(2) = 2
 *
 * F(2^t*k) = t*F(2k) - (t-1)*F(k)
 *
 * F(2^t*k + 1) = F(2^(t-1)*k)
 * F(2(2^t*k + 1)) = (2t-1)*F(2k) - (2t-3)*F(k)
 *
 * Find N such that given p/q, F(N) / F(N-1) = p/q
 *
 * Interestingly, note that the maximum value of counts for a given
 * number of bits reaches when the number is doubled or
 * doubled/add-2. The maximum value that is reached is the Fibonacci
 * number.
 */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

static u64 get_count (u64 N)
{
    if (N == 0 || N == 1)
        return 1;
    if (N == 2)
        return 2;
    if ((N & (N+1)) == 0)
        return 1;

    if (N & 1) {
        return get_count(N >> 1);

    } else if (N & 2) {
        int t = 0;

        N >>= 1;
        N--;
        while (!(N & 1)) {
            N >>= 1;
            t++;
        }
        return ((2*t-1)*get_count(N<<1) - (2*t-3)*get_count(N));
        
    } else {
        int t = 0;

        while (!(N & 1)) {
            N >>= 1;
            t++;
        }
        return (t*get_count(N<<1) - (t-1)*get_count(N));
    }
    return 0;
}

#define MAXN    (10000000ULL)

int main_1 (int argc, char *argv[])
{
    u64 c1, c2, c3;
    u64 N;
    u32 p, q;
    int i;

    while (scanf("%u %u", &p, &q) == 2) {

        /* Find a N such that F(N) <= p, F(2N) or F(2N+2) >= p */
        N = 2;
        i = 0;
        while (N <= ((1ULL << 63)-1)) {
            c1 = get_count(N);
            c2 = get_count(2*N);
            c3 = get_count(2*N+2);
            if (c1 <= p && (c2 >= p || c3 >= p)) {
                printf("Found F(%I64u) = %I64u, F(%I64u) = %I64u, "
                       "F(%I64u) = %I64u\n",
                       N, c1, 2*N, c2, 2*N+2, c3);
                fflush(stdout);
                break;
            }
            N <<= 1;
            if (i & 1) {
                N += 2;
            }
            i ^= 1;
            printf("I = %d, N = %I64u\n", i, N);
        }
    }
    return 0;
}

#define MAX_N  2000
static u64 visitN[MAX_N];

int main_2 (int argc, char *argv[])
{
    u64 N;
    u32 c, t;

    for (t = 0, N = 1; t < MAX_N && N < 1000000; N++) {
        c = get_count(N);
        if (c < MAX_N && !visitN[c]) {
            visitN[c] = N;
            t++;
        }
    }
    for (c = 0; c < MAX_N; c++) {
        printf("N = %4d counted at %I64u\n",
               c, visitN[c]);
    }
    return 0;
}
