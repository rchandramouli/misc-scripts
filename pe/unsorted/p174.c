/*
 * Square laminae, note that 4(T+1)(K-T) = N
 */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N    (1000000ULL)

static u32 tcount[1+MAX_N/4];

int main (int argc, char *argv[])
{
    u64 t, k, c, v, S;

    timeit_timer_start();
    for (c = t = 0; t < MAX_N/4; t++) {
        for (k = 2*t+2; k <= MAX_N/4; k++) {
            v = (t+1)*(k-t);
            if (v > MAX_N/4)
                break;
            c++;
            tcount[v]++;
        }
    }

    for (v = 0, t = 0; t <= MAX_N/4; t++) {
        if (tcount[t] == 15)
            v++;
    }
    for (S = 0, t = 0; t <= MAX_N/4; t++) {
        if (tcount[t] && tcount[t] <= 10)
            S++;
    }
    timeit_timer_peek_and_print();

    printf("Number of square laminae = %I64u\n", c);
    printf("Total count of first 10 types = %I64u (N(15) = %I64u)\n",
           S, v);

    return 0;
}
