#include <stdio.h>
#include <math.h>

#define MAX_PRIMES       10000
#define MAX_SIEVE_SIZE   (MAX_PRIMES*10)

static int nr_primes;
static int p_table[MAX_SIEVE_SIZE];
static char p_flag[MAX_SIEVE_SIZE];

static void generate_prime_table (void)
{
    int i, j;

    p_table[nr_primes++] = 2;

    for (i = 3; i < MAX_SIEVE_SIZE; i += 2) {
        if (p_flag[i])
            continue;
        p_table[nr_primes++] = i;
        for (j = 2*i; j < MAX_SIEVE_SIZE; j += i)
            p_flag[j] = 1;
    }
}

static int is_prime (int n);
static int is_prime_loop_f (int sq, int f, int n);

static int is_prime_loop_f (int sq, int f, int n)
{
    int i;

    if (!sq) {
        sq = sqrt(n);
    }
    for (i = f | 1; i <= sq; i += 2) {
        if ((n % i) == 0) {
            return i;
        }
    }
    return n;
}

static int is_prime (int n)
{
    if (!(n & 1)) {
        return 0;
    } else if (n < MAX_SIEVE_SIZE) {
        return !p_flag[n];
    } else {
        int sq = sqrt(n);
        int i;

        for (i = 0; i < nr_primes; i++) {
            if ((n % p_table[i]) == 0)
                return 0;
        }
        if (sq < p_table[nr_primes-1]) {
            if (is_prime_loop_f(sq, p_table[nr_primes-1] + 2, n) != n)
                return 0;
        }
    }
    return 1;
}

/* Returns index or -1 if out of range, fills up the prime divisible
 * by n
 */
static int get_pfactor (int n, int sq, int idx, int *p)
{
    int from_p = *p, i;

    if (idx >= 0) {
        for (i = idx; i < nr_primes; i++) {
            if ((n % p_table[i]) == 0) {
                *p = p_table[i];
                return i+1;
            }
        }
        idx = -1;
        from_p = p_table[nr_primes-1];
    }
    if (idx < 0) {
        i = is_prime_loop_f(sq, from_p + 2, n);
        if (i == n)
            *p = 0;
        idx = -1;
    }
    return idx;
}

static int get_totient (int n)
{
    int c, i, p, sq;

    if (is_prime(n)) {
        return (n-1);
    }

    c = n;
    sq = sqrt(n);
    i = 0;
    p = 1;

    do {
        i = get_pfactor(n, sq, i, &p);
        if (p) {
            c /= p;
            c *= (p-1);
        }
        //printf("\t%s: n = %d, p = %d, i = %d, c = %d\n",
        //__func__, n, p, i, c);
    } while (p != 0);

    return c;
}

int main (int argc, char *argv[])
{
    double max_ratio = 0.0, v;
    int n, t, max_n, max_totient;

    generate_prime_table();
    printf("Number of primes generated = %d\n", nr_primes);

    for (n = 2; n <= 1000000; n++) {
        t = get_totient(n);
        v = (double)n / t;
        if (v > max_ratio) {
            max_ratio = v;
            max_n = n;
            max_totient = t;
        }
    }

    printf("Maximum ratio = %lf, n = %d, totient = %d\n",
           max_ratio, max_n, max_totient);

    return 0;
}
