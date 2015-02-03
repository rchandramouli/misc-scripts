#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u64
#define MAX_PTABLE_SIZE (200000)
#include "prime.h"

static u64 dm[] = {
    0, 10, 100, 1000, 10000, 100000,
    1000000, 10000000, 100000000,
    1000000000
};
static u64 NN[] = {
    1, 11, 111, 1111, 11111, 111111,
    1111111, 11111111, 111111111,
    1111111111
};
static int plast[] = { 1, 3, 7, 9 };

static u64 get_digit_sum (int d, int runs, int nd)
{
    int i, j, k, l;
    u64 p, s = 0;

    if (runs > nd)
        return 0;

    switch (d) {
    case 0:
        if (runs > nd-2)
            return 0;
        k = runs+1;
        break;
    case 1:
    case 3:
    case 7:
    case 9:
        if (runs == nd) {
            if (is_prime(NN[nd-1]*d))
                return NN[nd-1]*d;
            return 0;
        }
        k = 1;
        break;
    case 2:
    case 4:
    case 5:
    case 6:
    case 8:
        if (runs == nd) {
            return 0;
        }
        k = 1;
        break;
    }
    for (l = nd-1; l >= k; l--) {
        for (i = 0; i <= 9; i++) {
            if (l == nd-1 && i == 0)
                continue;
            if (i == d && l < nd-1)
                continue;
            for (j = 0; j < 4; j++) {
                if (i == d && plast[j] == d)
                    continue;
                if (runs == nd-1 && i != d && plast[j] != d)
                    continue;
                p = (NN[nd-1]*d) + (i-d) * dm[l] + (plast[j]-d);
                if (is_prime(p)) {
                    printf("\tD = %d, RUNS = %d, POS = %2d:  prime = %I64d\n",
                           d, runs, l+1, p);
                    s += p;
                }
            }
        }
    }
    printf("\n\t\tSUM = %I64d\n\n", s);
    return s;
}

int main (int argc, char *argv[])
{
    int d, i;
    u64 S[10], Sa, v;

    generate_primes();

    for (Sa = d = 0; d <= 9; d++) {
        for (i = 10; i > 0; i--) {
            v = get_digit_sum(d, i, 10);
            if (v) {
                S[d] = v;
                break;
            }
        }
        Sa += S[d];
    }

    printf("Sum of all primes with max-runs = %I64d\n", Sa);
    return 0;
}

