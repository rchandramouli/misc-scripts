/* Check for numbers N < 10^8 such that, they are palindromes and
 * are expressible as sum of squares of consecutive numbers.
 *
 * Check if this expr: f(m, k) = km(m+k+1) + [k(k+1)(2k+1)/6]
 * is a palindrome
 */

#include <stdio.h>

#include "mytypes.h"

#define MAX_N5  (10000ULL)
#define MAX_N8  (100000000ULL)
#define MAX_N6  (1000000ULL)

static int is_palindrome (u64 n)
{
    u64 p = 0, t = n;

    while (t) {
        p = p*10 + (t % 10);
        t /= 10;
    }
    return (p == n);
}

static u32 palin_N[MAX_N6];

int main (int argc, char *argv[])
{
    u64 m, k, pdt, S;
    u64 min_pdt = MAX_N8, min_m, min_k;
    u64 max_pdt = 0, max_m, max_k;
    int i;

    for (m = 0; m < MAX_N5; m++) {
        for (k = 2; k < MAX_N5; k++) {
            pdt = k*m*(m+k+1) + (k*(k+1)*(2*k+1))/6;

            if (pdt < MAX_N8 && is_palindrome(pdt)) {
                if (palin_N[pdt % MAX_N6]) {
                    printf("Duplicate!! M = %I64u, K = %I64u, "
                           "N = %I64u (%u)\n", m, k, pdt,
                           palin_N[pdt % MAX_N6]);
                    continue;
                }
                printf("M = %I64u, K = %I64u, N = %I64u\n", m, k, pdt);

                palin_N[pdt % MAX_N6] = pdt;

                if (pdt < min_pdt) {
                    min_pdt = pdt;
                    min_m = m;
                    min_k = k;
                }
                if (pdt > max_pdt) {
                    max_pdt = pdt;
                    max_m = m;
                    max_k = k;
                }
            }
        }
    }
    for (i = S = 0; i < MAX_N6; i++) {
        if (palin_N[i])
            S += palin_N[i];
    }
    printf("\nSum of all palindromes = %I64u\n", S);
    printf("\nMinumum: M = %I64u, K = %I64u, N = %I64u"
           "\nMaximum: M = %I64u, K = %I64u, N = %I64u\n",
           min_m, min_k, min_pdt, max_m, max_k, max_pdt);

    return 0;
}
