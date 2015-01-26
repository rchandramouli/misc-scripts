/* TBD */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u32
#define MAX_PTABLE_SIZE  (1000000)
#include "prime.h"

#define MAX_N   (1000000000000ULL)

int main (int argc, char *argv[])
{
    u64 pwr2, pwr5, n1, n2, n3;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    n1 = 2;
    pwr2 = 0;
    while (n1 <= MAX_N) {
        pwr2 += MAX_N / n1;
        n1 *= 2;
    }

    n1 = 5;
    pwr5 = 0;
    while (n1 <= MAX_N) {
        pwr5 += MAX_N / n1;
        n1 *= 5;
    }

    pwr2 -= pwr5;

    return 0;
}
