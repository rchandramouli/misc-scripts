#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N   10000000

static int divcount[MAX_N];

int main (int argc, char *argv[])
{
    int c, i, j, v;

    timeit_timer_start();
    for (i = 1; i < MAX_N; i++)
        divcount[i] = 1 + ((i & 1) ^ 1);
    timeit_timer_peek_and_print();

    for (i = 3; i < MAX_N; i++)
        for (j = i; j < MAX_N; j += i)
            divcount[j]++;
    timeit_timer_peek_and_print();

    for (c = 0, i = 1; i < MAX_N-1; i++) {
        if (divcount[i] == divcount[i+1])
            c++;
    }
    timeit_timer_peek_and_print();

    printf("Number of pair numbers = %d\n", c);

    return 0;
}
