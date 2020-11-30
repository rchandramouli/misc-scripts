/* TODO - takes hell-a-lota time, remember the 1min rule. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE (20000000ULL)
//#define MAX_PTABLE_SIZE (2000ULL)
#include "prime.h"

#ifdef DEBUG
#define dprintf printf
#else
#define dprintf(...)
#endif

static u64 get_max_modular_inverse (u64 num)
{
    long double v1, v2, sqn;
    u64 a = 0, i;

    sqn = sqrtl((long double)num);

    for (i = 2; i < num; i++) {
	//v1 = sqrtl((long double)i)*sqn;
	//v2 = sqrtl((long double)num*i+1.0);
	a = sqrtl((long double)i)*sqn;
	if ((a*(a+2)) == (num*i))
	    break;
    }

    if (a) {
	if ((((a+1)*(a+1)) % num) != 1ULL) {
	    printf("FAIL::: N: %"PRIu64" (%"PRIu64" -- %"PRIu64") "
		   "(%"PRIu64" --- %"PRIu64") M: %"PRIu64"\n",
		   num, a+1, (a+1)*(a+1), num-a-1, (num-a-1)*(num-a-1),
		   (((a+1)*(a+1)) % num));
	    printf("FAIL::: (%Lf --- %Lf)\n\n", v1, v2);
	}
	if (a+1 == num-1) {
	    a = 0;
	} else if ((a+1) < num/2) {
	    a = num-a-2;
	}
    }

    return (a+1);
}

int main (int argc, char *argv[])
{
    u64 S, i, j, v;

    timeit_timer_start();
    generate_primes();
    printf("Generated %"PRIu64"\n", nr_primes);
    timeit_timer_peek_and_print();

    /* For all primes, 1^1 = 1 (mod p), skip 2 since n >= 3 */
    S = nr_primes - 1;

    for (i = 1; i < nr_primes-1; i++) {
        if ((i % 30000) == 0) {
            timeit_timer_peek_and_print();
            printf("DONE for %"PRIu64" primes\n", i);
            fflush(stdout);
        }
        for (j = ptable[i]+1; j <= ptable[i+1]-1; j++) {
            v = get_max_modular_inverse(j);
            dprintf("%"PRIu64": %"PRIu64"\n", j, v);
            S += v;
        }
    }
    for (j = ptable[nr_primes-1]+1; j <= MAX_PTABLE_SIZE; j++) {
        v = get_max_modular_inverse(j);
        dprintf("%I"PRIu64": %"PRIu64"\n", j, v);
        S += v;
    }
    timeit_timer_peek_and_print();

    printf("Modular inverse sum = %"PRIu64"\n", S);

    return 0;
}
