/*
 * F(0) = F(1) = 1, F(2) = 2
 *
 * F(2^t*k) = t*F(2k) - (t-1)*F(k)
 *
 * F(2^t*k + 1) = F(2^(t-1)*k)
 * F(2(2^t*k + 1)) = (2t-1)*F(2k) - (2t-3)*F(k)
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

#define N_5P25  (298023223876953125ULL)
#define N_2P25  (25)

int main (int argc, char *argv[])
{
    u64 N;

    N = N_2P25*get_count(N_5P25 << 1ULL) - (N_2P25-1)*get_count(N_5P25);

    printf("Count(10^25) = %I64u\n", N);

    return 0;
}

