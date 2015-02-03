#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N  (1000000)

static u32 divs[MAX_N*10];
static u32 visits[MAX_N*10];

int main (int argc, char *argv[])
{
    u32 i, j, k, n;
    u32 max = 0, l = 0, f;

    timeit_timer_start();

    for (i = 1; i < MAX_N; i++)
        divs[i] = 1;

    for (i = 2; i < MAX_N; i++)
        for (j = 2*i; j < MAX_N; j += i)
            divs[j] += i;

    timeit_timer_peek_and_print();

    /* Now find the chains */
    for (i = 1, n = 1; i < MAX_N; i++) {
        if (visits[i])
            continue;

        k = f = 0;
        j = i;
        while (!visits[j]) {
            k++;
            visits[j] = n;
            j = divs[j];
            if (j >= MAX_N)
                f = 1;
        }
        /* If it is the same chain */
        if (!f && visits[j] == n) {
            if (max < k) {
                max = k;
                l = j;
            }
        }
        n++;
    }
    timeit_timer_peek_and_print();

    printf("Ring length = %lu, elem = %lu\n", max, l);

    /* Find minimum element */
    i = j = l;
    printf("%lu ", j);
    do {
        j = divs[j];
        if (i > j)
            i = k;
        printf("--- %lu ", j);
    } while (j != l);
    printf("\n");

    timeit_timer_peek_and_print();

    printf("Maximum ring length = %lu, least element = %lu\n",
           max, i);

    return 0;
}
