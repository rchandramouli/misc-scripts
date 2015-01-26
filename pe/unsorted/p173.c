/*
 * Square laminae, note that 4(T+1)(K-T) = N
 */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N    (1000000ULL)

int main (int argc, char *argv[])
{
    u64 t, k, c, v;

    timeit_timer_start();
    for (c = t = 0; t < MAX_N/4; t++) {
        for (k = 2*t+2; k <= MAX_N/4; k++) {
            v = (t+1)*(k-t);
            if (v > MAX_N/4)
                break;
            c++;
        }
    }
    timeit_timer_peek_and_print();
    printf("Number of square laminae = %I64u\n", c);

    return 0;
}
