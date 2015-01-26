#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u32
#define MAX_PTABLE_SIZE   (4000000UL)
#include "prime.h"

#define MAX_N        (40000000UL)
#define P_CHAIN_LEN  (25)

static u32 ntot[MAX_N+1];
static u32 nchain[MAX_N+1];

static u32 compute_totient_chain (u32 n)
{
    if (nchain[n])
        return nchain[n];

    nchain[n] = 1 + compute_totient_chain(ntot[n]);

    return nchain[n];
}

int main (int argc, char *argv[])
{
    u32 n, k, v, pc;
    u64 ps;

    timeit_timer_start();

    generate_primes();
    printf("Number of primes = %lu, max-prime = %lu\n",
           nr_primes, ptable[nr_primes-1]);
    timeit_timer_peek_and_print();

    nchain[1] = 1;

    /* Compute totient of numbers */
    for (n = 1; n <= MAX_N; n++)
        ntot[n] = n;
    for (n = 0; n < nr_primes && ptable[n] < MAX_N; n++) {
        k = 1;
        v = ptable[n];
        while (v <= MAX_N) {
            ntot[v] /= ptable[n];
            ntot[v] *= (ptable[n]-1);
            k++;
            v = k*ptable[n];
        }
    }
    timeit_timer_peek_and_print();

    /* Compute chains */
    for (n = pc = ps = 0; n < nr_primes && ptable[n] < MAX_N; n++) {
        if (compute_totient_chain(ptable[n]) == P_CHAIN_LEN) {
            pc++;
            ps += ptable[n];
        }
    }
    timeit_timer_peek_and_print();

    printf("Primes with chain-length = %lu is %lu, Sum = %I64u (< %lu)\n",
           P_CHAIN_LEN, pc, ps, MAX_N);

    return 0;
}
