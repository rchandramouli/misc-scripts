/* TBD */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE   (10000000ULL)
#include "prime.h"

static u64 get_totient_nonprime (u32 N)
{
    u32 i;
    u64 r = N;

    for (i = 0; r > 1 && i < nr_primes; i++) {
        if ((r % ptable[i]) == 0) {
            while (!(r % ptable[i]))
                r /= ptable[i];
            N /= ptable[i];
            N *= (ptable[i]-1);
        }
    }
    return N;
}

#define CONST_K1   (15499ULL)
#define CONST_K2   (94744ULL)

int main (int argc, char *argv[])
{
    u64 i, n, d;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    for (i = 1; i < 10000; i++) {
        d = CONST_K2*i + 1;
        if (is_prime(d))
            continue;
        n = get_totient_nonprime(d);
        if ((n % i) == 0 && n/i == CONST_K1) {
            printf("R(%I64u) = %I64u / %I64u (K1 / K2 = %I64u / %I64u)\n",
                   d, n, d-1, CONST_K1, CONST_K2);
            break;
        }
    }
    timeit_timer_peek_and_print();

    return 0;
}
