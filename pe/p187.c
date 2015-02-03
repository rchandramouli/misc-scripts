#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N   100000000ULL

#define PTYPE_T_DEFINED
#define MAX_PTABLE_SIZE (MAX_N/2)
#define ptype_t   u64
#include "prime.h"

int main (int argc, char *argv[])
{
    u64 c, i, j;

    timeit_timer_start();
    generate_primes();
    timeit_timer_peek_and_print();

    for (c = i = 0; i < nr_primes; i++) {
        for (j = i; j < nr_primes; j++) {
            if (ptable[i]*ptable[j] >= MAX_N)
                break;
            c++;
        }
    }
    timeit_timer_peek_and_print();

    printf("Number of composites with 2 factors = %I64d\n", c);

    return 0;
}
