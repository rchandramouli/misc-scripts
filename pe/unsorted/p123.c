#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE  (1000000ULL)

#include "prime.h"

#define MAX_N9   (1000000000ULL)
#define MAX_N10  (10000000000ULL)
#define MAX_N11  (100000000000ULL)

int main (int argc, char *argv[])
{
    char r9 = 0, r10 = 0, r11 = 0;
    u32 i;
    u64 r, p;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    /* Start from 7035, to find the remainder that exceeds 10^10 */
    for (i = 7034; i < nr_primes; i += 2) {
        p = ptable[i];
        r = (2*(i+1)*p) % (p*p);
        if (r >= MAX_N9 && !r9) {
            printf("R > 10^9 at: index = %lu, P = %I64u, R = %I64u\n",
                   i+1, p, r);
            r9 = 1;
        }
        if (r >= MAX_N10 && !r10) {
            printf("R > 10^10 at: index = %lu, P = %I64u, R = %I64u\n",
                   i+1, p, r);
            r10 = 1;
        }
        if (r >= MAX_N11 && !r11) {
            printf("R > 10^11 at: index = %lu, P = %I64u, R = %I64u\n",
                   i+1, p, r);
            r11 = 1;
            break;
        }
    }
    timeit_timer_peek_and_print();

    return 0;
}
