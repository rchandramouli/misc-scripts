#include <stdio.h>
#include <math.h>

#define MAX_PRIMES    1000

static char p_flag[MAX_PRIMES];
static int p_table[MAX_PRIMES], nr_primes;

static void p_sieve (int max_n)
{
    int i, j, k;

    for (i = 2, k = 0; i < max_n; i++) {
        if (p_flag[i]) continue;

        p_table[k] = i;
        k++;

        for (j = i+i; j < max_n; j += i)
            p_flag[j] = 1;
    }
    nr_primes = k;
}

static int is_prime (int n)
{
    int l, i;

    if (n < 0)
        n = -n;

    if (n == 0 || n == 1)
        return 0;

    if (n < MAX_PRIMES) {
        return !p_flag[n];
    } else {
        l = sqrt(n);
        for (i = 0; i < nr_primes; i++) {
            if ((n % p_table[i]) == 0)
                return 0;
        }
        for (i = p_table[nr_primes-1] + 2; i <= l; i+= 2) {
            if ((n % i) == 0)
                return 0;
        }
    }
    return 1;
}

static int get_prime_coeffs (int a, int b)
{
    int n, r, v, c;

    r = (b < 0) ? -b: b;

    for (n = 0, c = 0; n < r; n++) {
        v = n*(n+a)+b;
        if (is_prime(v)) {
            c++;
        } else {
            break;
        }
    }
    return c;
}

int main (int argc, char *argv[])
{
    int i, j, ma, mb, c, k;

    p_sieve(MAX_PRIMES);

    c = 0;
    for (i = -999; i < 1000; i++) {
        for (j = 0; j < nr_primes; j++) {
            k = get_prime_coeffs(i, p_table[j]);
            if (k > c) {
                c = k;
                ma = i;
                mb = p_table[j];
            }
            k = get_prime_coeffs(i, -p_table[j]);
            if (k > c) {
                c = k;
                ma = i;
                mb = -p_table[j];
            }
        }
    }

    printf("n^2 + (%d)n + (%d) produces %d primes (pdt = %d)\n",
           ma, mb, c, ma*mb);
    return 0;
}
