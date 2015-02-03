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

#define MAX_ABLIST    28124

static int abundant_summables[MAX_ABLIST];
static int ablist[MAX_ABLIST];
static int nr_ablist;

static int is_abundant (int n)
{
    if (is_prime(n)) {
        return 0;
    } else {
        int s = 1, i;

        for (i = 2; i <= n/2; i++) {
            if ((n % i) == 0)
                s += i;
            if (s > n)
                break;
        }
        if (s > n) {
            return 1;
        }
    }
    return 0;
}

static void scan_all_abundant_summable (void)
{
    int i, j, v;

    for (i = 0; i < nr_ablist; i++) {
        for (j = i; j < nr_ablist; j++) {
            v = ablist[i] + ablist[j];
            if (v >= MAX_ABLIST)
                break;
            abundant_summables[v]++;
        }
    }
}

int main (int argc, char *argv[])
{
    int n, s;

    generate_prime_table();

    for (n = 12; n < MAX_ABLIST; n++) {
        if (is_abundant(n)) {
            ablist[nr_ablist++] = n;
        }
    }

    scan_all_abundant_summable();

    s = 0;
    for (n = 1; n < MAX_ABLIST; n++) {
        if (!abundant_summables[n]) {
            printf("%s: %d not-summable\n", __func__, n);
            s += n;
        }
    }

    for (n = 0; n < 100; n++) {
        if ((n % 10) == 0) printf("\n");
        printf("%4d ", ablist[n]);
    }

    printf("\n\n");
    for (n = nr_ablist-1; n >= nr_ablist-100; n--) {
        if ((n % 10) == 0) printf("\n");
        printf("%5d ", ablist[n]);
    }
    printf("\n");

    printf("Number of abundant numbers < 28124 = %d\n", nr_ablist);
    printf("Sum of Non-abundant summable numbers = %d\n", s);

    return 0;
}
