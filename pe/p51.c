/* TBD */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t  u64
#define MAX_PTABLE_SIZE (100000000ULL)

#include "prime.h"

int main (int argc, char *argv[])
{
    u64 n, i;
    int x[] = { 2, 4, 2, 2 }, r = 0;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    n = 0;
    
    for (i = MAX_PTABLE_SIZE+1; i < MAX_PTABLE_SIZE*2; i += x[r], r++)
        if (is_prime(i))
            n++;
    timeit_timer_peek_and_print();

    printf("Number of 8-digit primes = %I64u\n", n);

    return 0;
}
