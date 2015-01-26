/* Solve the equation: (8r+3) = 0 (mod p), S(P) = r
 *
 * Note that, we need some multiple of p such that p-3 is a multiple
 * of 8. To do this, see that we only need the last 4 bits to qualify
 * if the number is a multiple of 8. Sort the prime in one of the 8
 * bins and multiply to find the remainder.
 */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u32
#define MAX_PTABLE_SIZE  (10000000ULL)

#include "prime.h"

int main (int argc, char *argv[])
{
    u64 S = 0, k, v;
    int i, m;
    int pdt[16] = { 0, 3, 0, 1, 0, 7, 0, 5, 0, 3, 0, 1, 0, 7, 0, 5 };

    timeit_timer_start();

    generate_primes();
    printf("Number of primes = %u\n", nr_primes);
    printf("Last prime = %u (10^7 = 10000000)\n", ptable[nr_primes-1]);
    timeit_timer_peek_and_print();

    S = 4 + 4; /* p=5,7 */
    for (i = 4; i < nr_primes; i++) {
        v = ptable[i];
        v = ((v * pdt[v & 0xF])-3) / 8;
        S += v;
    }
    printf("Sum = %I64u\n", S);

    return 0;
}
