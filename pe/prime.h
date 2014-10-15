#ifndef PRIME_H
#define PRIME_H

#ifndef MAX_PTABLE_SIZE
#define MAX_PTABLE_SIZE  30000
#endif

#ifndef MAX_SIEVE_SIZE
#define MAX_SIEVE_SIZE   (MAX_PTABLE_SIZE * 10)
#endif

#ifndef PTYPE_T_DEFINED
#define ptype_t int
#endif

static ptype_t ptable[MAX_PTABLE_SIZE], nr_primes;
static char pflag[MAX_SIEVE_SIZE];

static void __attribute__((unused)) generate_primes (void)
{
    ptype_t i, j;

    ptable[nr_primes++] = 2;

    for (i = 3; i < MAX_SIEVE_SIZE; i += 2) {
        if (pflag[i])
            continue;
        ptable[nr_primes++] = i;
        for (j = 2*i; j < MAX_SIEVE_SIZE; j += i)
            pflag[j] = 1;
        if (nr_primes >= MAX_PTABLE_SIZE)
            break;
    }
}

static int __attribute__((unused)) is_prime (ptype_t n)
{
    ptype_t i, t;

    if (!(n & 1))
        return 0;

    if (n < MAX_PTABLE_SIZE)
        return !pflag[n];

    for (i = 0; i < nr_primes; i++)
        if ((n % ptable[i]) == 0)
            return 0;

    t = sqrtl(n);
    for (i = ptable[nr_primes-1] + 2; i <= t; i += 2)
        if ((n % i) == 0)
            return 0;
    return 1;
}
#endif /* PRIME_H */
