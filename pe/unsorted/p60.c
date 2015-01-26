#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_PTABLE_SIZE (1200)
#define PTYPE_T_DEFINED
#define ptype_t u64

#include "prime.h"

static int concat_and_check_prime (u64 p1, u64 p2)
{
    u64 d, n, v;

    for (v = p2, d = p1; d; d /= 10)
        v *= 10;
    v += p1;

    if (!is_prime(v))
        return 0;

    for (v = p1, d = p2; d; d /= 10)
        v *= 10;
    v += p2;

    if (!is_prime(v))
        return 0;

    return 1;
}

#define CHECK_PRIME_LIST2(n1, n2)                       \
    (concat_and_check_prime(ptable[n1], ptable[n2]))

#define CHECK_PRIME_LIST3(n1, n2, n3)                   \
    (concat_and_check_prime(ptable[n1], ptable[n3]) &&  \
     concat_and_check_prime(ptable[n2], ptable[n3]))

#define CHECK_PRIME_LIST4(n1, n2, n3, n4)               \
    (concat_and_check_prime(ptable[n1], ptable[n4]) &&  \
     concat_and_check_prime(ptable[n2], ptable[n4]) &&  \
     concat_and_check_prime(ptable[n3], ptable[n4]))

#define CHECK_PRIME_LIST5(n1, n2, n3, n4, n5)           \
    (concat_and_check_prime(ptable[n1], ptable[n5]) &&  \
     concat_and_check_prime(ptable[n2], ptable[n5]) &&  \
     concat_and_check_prime(ptable[n3], ptable[n5]) &&  \
     concat_and_check_prime(ptable[n4], ptable[n5]))
                                      
#define FOR(v, b, e)                              \
    for (v = (b); !found && v < (e); v++)

static u64 sample_4numbers (void)
{
    int n1, n2, n3, n4, found = 0;
    u64 sum = -1, s;

    FOR(n1, 1, nr_primes) {
        FOR(n2, n1+1, nr_primes) {
            if (!CHECK_PRIME_LIST2(n1, n2))
                continue;

            FOR(n3, n2+1, nr_primes) {
                if (!CHECK_PRIME_LIST3(n1, n2, n3))
                    continue;

                FOR(n4, n3+1, nr_primes) {
                    if (!CHECK_PRIME_LIST4(n1, n2, n3, n4))
                        continue;

                    s = ptable[n1] + ptable[n2] + ptable[n3] + ptable[n4];
                    if (s < sum)
                        sum = s;

                    printf("Prime list (%d, %d, %d, %d), sum = %d\n",
                           ptable[n1], ptable[n2], ptable[n3], ptable[n4],
                           s);
                }
            }
        }
    }
    return sum;
}

static u64 sample_5numbers (void)
{
    int n1, n2, n3, n4, n5, found = 0;
    u64 sum = -1, s;

    FOR(n1, 1, nr_primes) {
        FOR(n2, n1+1, nr_primes) {
            if (!CHECK_PRIME_LIST2(n1, n2))
                continue;

            FOR(n3, n2+1, nr_primes) {
                if (!CHECK_PRIME_LIST3(n1, n2, n3))
                    continue;

                FOR(n4, n3+1, nr_primes) {
                    if (!CHECK_PRIME_LIST4(n1, n2, n3, n4))
                        continue;

                    FOR(n5, n4+1, nr_primes) {
                        if (!CHECK_PRIME_LIST5(n1, n2, n3, n4, n5))
                            continue;

                        s = ptable[n1] + ptable[n2] + ptable[n3] +
                            ptable[n4] + ptable[n5];

                        if (s < sum)
                            sum = s;

                        printf("Prime list (%d, %d, %d, %d, %d), sum = %d\n",
                               ptable[n1], ptable[n2], ptable[n3],
                               ptable[n4], ptable[n5], s);
                    }
                }
            }
        }
    }
    return sum;
}

int main (int argc, char *argv[])
{
    timeit_timer_start();
    generate_primes();
    timeit_timer_peek_and_print();
    fflush(stdout);

    sample_4numbers();
    timeit_timer_peek_and_print();
    fflush(stdout);

    sample_5numbers();
    timeit_timer_peek_and_print();
    fflush(stdout);

    return 0;
}
