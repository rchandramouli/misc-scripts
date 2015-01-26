#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE (10000000ULL)
#include "prime.h"

#ifndef INPUT_N
#define INPUT_N  (10000000ULL)
#endif

#define MOD_N   (1000000007ULL)

int main (int argc, char *argv[])
{
    u64 ncr = 1;

    timeit_timer_start();
    generate_primes();
    printf("Generated %I64d primes\n", nr_primes);
    timeit_timer_peek_and_print();

    /* Compute C(2N, N) mod MOD_N. Note: MOD_N is prime */
    

    return 0;
}
