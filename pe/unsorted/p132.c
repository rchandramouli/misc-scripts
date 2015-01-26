#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u32
#define MAX_PTABLE_SIZE   (10000000UL)
#include "prime.h"

//#define MAX9_N (10)
//#define MAX_P (4)
#define MAX9_N   (1000000000UL)
#define MAX_P  (40)

static int is_prime_a_factor (u32 n)
{
    u32 pdt = 10, k = 1;

    while (pdt != 1 && k < n) {
        pdt = (pdt * 10) % n;
        k++;
    }
    if (pdt != 1 || ((MAX9_N % k) != 0))
        return 0;

    if ((k % 3) != 0)
        return ((n % 3) != 0);

    return 1;
}

int main (int argc, char *argv[])
{
    u32 p, i;
    u64 s;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    for (i = p = s = 0; i < nr_primes && p < MAX_P; i++) {
        if (is_prime_a_factor(ptable[i])) {
            printf("Factor = %lu\n", ptable[i]);
            s += ptable[i];
            p++;
        }
    }

    printf("Sum = %I64u\n", s);
    timeit_timer_peek_and_print();

    return 0;
}
