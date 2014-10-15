#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE (50000000ULL)
#include "prime.h"

#define MAX_SAMPLE_SIZE (100000000ULL)

static char special_nos[MAX_SAMPLE_SIZE+1];

int main (int argc, char *argv[])
{
    u64 i, j, S;
    u64 n;

    timeit_timer_start();
    generate_primes();
    printf("Generated %I64d primes\n", nr_primes);
    timeit_timer_peek_and_print();

    /* Mark prime-1 as special */
    for (i = 0; i < nr_primes && ptable[i] <= MAX_SAMPLE_SIZE+1; i++)
        special_nos[ptable[i]-1] = 1;
    timeit_timer_peek_and_print();

    for (i = 2; i <= MAX_SAMPLE_SIZE/2; i++) {
        if ((i % 10000000) == 0) {
            printf("I = %I64d\n", i);
            timeit_timer_peek_and_print();
        }
        for (j = i; j <= MAX_SAMPLE_SIZE; j += i) {
            if (!special_nos[j])
                continue;
            if (!is_prime(i+j/i))
                special_nos[j] = 0;
        }
    }
    timeit_timer_peek_and_print();

    for (n = S = 0, i = 1; i <= MAX_SAMPLE_SIZE; i++) {
        if (special_nos[i]) {
            S += i;
            n++;
        }
    }
    /* Print last 100 special numbers */
    for (i = 0, j = MAX_SAMPLE_SIZE; j > 0 && i < 100; j--) {
        if (special_nos[j]) {
            i++;
            printf("[%I64d] %I64d\n", i, j);
        }
    }
    printf("Sum of special numbers = %I64d (total = %I64d)\n", S, n);
    timeit_timer_peek_and_print();

    return 0;
}
