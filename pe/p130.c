#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t  u32
#define MAX_PTABLE_SIZE (1000000UL)
#include "prime.h"

static int is_rep_composite (u32 n)
{
    u32 pdt = 10, k = 1;

    while (pdt != 1 && k < n) {
        pdt = (pdt * 10) % n;
        k++;
    }
    if (pdt != 1 || ((n % k) != 1))
        return 0;

    if ((k % 3) != 0)
        return ((n % 3) != 0);

    return 1;
}

int main (int argc, char *argv[])
{
    u32 n, s = 0, c = 0;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    for (n = 6; c < 25; n++) {
        if (is_rep_composite(n) && !is_prime(n)) {
            printf("REP composite = %lu\n", n);
            s += n;
            c++;
        }
    }
    printf("Sum = %lu\n", s);

    return 0;
}
