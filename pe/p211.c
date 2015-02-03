#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "timeit.h"

#define MAX_N    64000000ULL

typedef unsigned long long int u64;

static u64 sqr[MAX_N];

int main (int argc, char *argv[])
{
    u64 S, t;
    u64 i, j, c;

    timeit_timer_start();
    for (i = 1; i < MAX_N; i++) {
        for (j = i; j < MAX_N; j += i) {
            sqr[j] += i*i;
        }
    }
    timeit_timer_peek();
    timeit_timer_print_elapsed();

    timeit_timer_start();
    for (c = 0, S = 0, i = 1; i < MAX_N; i++) {
        t = sqrt(sqr[i]);
        if (t*t == sqr[i]) {
            printf("\tSQR = %I64d, I = %I64d, t = %I64d\n",
                   sqr[i], i, t);
            c++;
            S += i;
        }
    }
    timeit_timer_peek();
    timeit_timer_print_elapsed();

    printf("S = %I64d, c = %I64d\n", S, c);

    return 0;
}
