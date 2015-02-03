/* TBD */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_PTABLE_SIZE  (1000000)
#include "prime.h"

#define dprintf(...)

#define MAX_N   120000
//#define MAX_N   10000

static u32 rad_n[MAX_N+1];

static void calculate_rad_all (void)
{
    int i, j;

    for (i = 1; i <= MAX_N; i++)
        rad_n[i] = 1;
    for (i = 0; i < nr_primes; i++) {
        if (ptable[i] > MAX_N)
            break;
        for (j = ptable[i]; j <= MAX_N; j += ptable[i]) {
            rad_n[i] *= ptable[i];
        }
    }
}

static int gcd (int a, int b)
{
    int t;

    if (a > b) {
        t = a;
        a = b;
        b = t;
    }
    while (a) {
        t = a;
        a = b%a;
        b = t;
    }
    return b;
}

static int unitary_gcd3_and_rad_met (int a, int b, int c)
{
    u64 pdt;

    if (gcd(a, b) != 1)
        return 0;

    pdt = rad_n[a]*rad_n[b]*rad_n[c];
    return (pdt < c);
}

int main (int argc, char *argv[])
{
    int a, b, c;
    u64 S, n;

    timeit_timer_start();
    generate_primes();
    timeit_timer_peek_and_print();
    fflush(stdout);

    calculate_rad_all();
    timeit_timer_peek_and_print();
    fflush(stdout);

    for (n = S = 0, a = 1; a < MAX_N; a++) {
        for (b = a+1; b < MAX_N-a; b++) {
            c = a+b;
            if (!unitary_gcd3_and_rad_met(a, b, c))
                continue;
            S += c;
            n++;
            dprintf("abc-hit on (%d, %d, %d)\n", a, b, c);
            if ((n % 1000) == 0) {
                printf("%s: hit %d abc-hits!\n", __func__, n);
                timeit_timer_peek_and_print();
                fflush(stdout);
            }
        }
    }
    printf("Number of abc-hits = %I64d, Sum = %I64d\n", n, S);
    timeit_timer_peek_and_print();

    return 0;
}
