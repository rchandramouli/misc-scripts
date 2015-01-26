/*
 * Number of solutions to Diophantine equation:
 *       1/x + 1/y = 1/n
 * TBD
 */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_PTABLE_SIZE   (100000)
#define PTYPE_T_DEFINED
#define ptype_t u64

#include "prime.h"

static int count_solutions (u64 n)
{
    u64 i;
    int c = 2;

    if (is_prime(n))
        return c;

    for (i = n+2; i < 2*n; i++) {
        if (((n*i) % (i-n)) == 0)
            c++;
    }
    return c;
}

int main (int argc, char *argv[])
{
    u64 i;
    int j, m;

    generate_primes();

    i = 1;
    m = 2;
    while (1) {
        j = count_solutions(i++);
        if (m < j) {
            m = j;
            printf("m = %d at N = %I64d\n", m, i-1);
            fflush(stdout);
        }
        if (j >= 1000)
            break;
    }
    printf("\nMax soln. for n = %I64d count = %d\n", i-1, j);
    return 0;
}
