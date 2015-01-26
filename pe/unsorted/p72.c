#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t  u32
#define MAX_PTABLE_SIZE (100000)

#include "prime.h"
#define MAX_N  (1000001)
//#define MAX_N  (9)

static int div[MAX_N];

int main (int argc, char *argv[])
{
    int i, j;
    u32 p;
    u64 S = 0;

    timeit_timer_start();

    generate_primes();
    printf("Number of primes = %lu, last prime = %lu\n",
           nr_primes, ptable[nr_primes-1]);
    timeit_timer_peek_and_print();

    for (i = 2; i < MAX_N; i++)
        div[i] = i;
    for (i = 0; i < nr_primes && ptable[i] < MAX_N; i++) {
        p = ptable[i];
        for (j = p; j < MAX_N; j += p) {
            div[j] /= p;
            div[j] *= (p-1);
        }
    }
    timeit_timer_peek_and_print();

    for (i = 2; i < MAX_N; i++) {
        S += div[i];
    }
    timeit_timer_peek_and_print();

    printf("Total fractions = %I64u\n", S);

    return 0;
}
