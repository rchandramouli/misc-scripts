#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u32
#define MAX_PTABLE_SIZE (10000000UL)
#include "prime.h"

#define MAX_N  (100000)

static int is_2n5n (u32 n)
{
    while (!(n & 1))
        n >>= 1;
    while (!(n % 5))
        n /= 5;
    return (n == 1);
}

static int is_rep_factor (u32 n)
{
    u32 pdt = 10, k = 1;

    while (pdt != 1 && k < n) {
        pdt = (pdt * 10) % n;
        k++;
    }
    return is_2n5n(k);
}

int main (int argc, char *argv[])
{
    u64 s;
    u32 i;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    s = 2 + 3 + 5;
    for (i = 3; i < nr_primes && ptable[i] < MAX_N; i++) {
        if (is_rep_factor(ptable[i])) {
            printf("Factor = %lu\n", ptable[i]);
        } else {
            s += ptable[i];
        }
    }

    timeit_timer_peek_and_print();
    printf("Non-factor Sum = %I64u\n", s);

    return 0;
}
