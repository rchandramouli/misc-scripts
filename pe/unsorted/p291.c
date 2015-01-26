/* TBD */
/*
 * Primes, p = (x^4 - y^4) / (x^3 + y^3)
 *
 *    p = (x - y) - xy(x - y) / (x^2 - xy + y^2)
 *
 * U = x+y, V = x-y, p = 2V(U^2 + V^2)/(U^2 + 3V^2),
 *
 * Set, 2V = p+1, U = kU (for some k), we have, p = (k^2 + 1)/2,
 *
 *       Set k = 2r+1, p = (4r^2 + 4r + 2)/2 = r^2 + (r+1)^2
 *
 * Or, 4T(n)+1 is a prime (where T(n) is a triangular number)
 */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE   (8000000ULL)
//#define MAX_PTABLE_SIZE   (800000ULL)
#include "prime.h"

#define MAX_P   (5000000000000000ULL)
//#define MAX_P   (50000ULL)

int main (int argc, char *argv[])
{
    u64 sq, s, n;
    u32 i;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    n = 1;
    s = 0;
    sq = 5;

    /* Check all primes once */
    s++; /* p = 2 works */
    for (i = 1; i < 300; /* nr_primes */i++) {
        if ((ptable[i] % 10) == 1 || (ptable[i] % 10) == 3)
            continue;
        //if ((ptable[i] & 3) != n)
        //continue;
        sq = 2*ptable[i]*(ptable[i]+1) + 1;
        if (is_prime(sq)) {
            s++;
            n ^= 2;
            //printf("%3I64u (%d)\n", ptable[i], ptable[i] & 0x3);
            printf("%d", ptable[i] & 0x3);
        }
    }
    timeit_timer_peek_and_print();
    printf("Number of Panaitopol primes with N as prime = %I64u (< %I64u)\n",
           s, MAX_P);
#if 0
    while (sq < MAX_P) {
        if ((n % 50000) == 0) {
            printf("n = %I64u\n", n);
            timeit_timer_peek_and_print();
        }
        if (is_prime(sq)) {
            s++;
        }
        n++;
        switch (n % 10) {
        case 1:
        case 3:
        case 6:
        case 8:
            n++;
            break;
        }
        sq = n*n + (n+1)*(n+1);
    }
    timeit_timer_peek_and_print();
    printf("Number of Panaitopol primes < %I64u = %I64u\n", MAX_P, s);
#endif
    return 0;
}
