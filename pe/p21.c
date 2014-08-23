#include <stdio.h>
#include <math.h>

#define MAX_PRIMES   1000

static int prime[MAX_PRIMES];
static char is_prime[MAX_PRIMES];
static int n_prime;

static void build_prime_table (void)
{
    int a, i, j;

    prime[0] = 2;
    prime[1] = 3;
    n_prime = 2;

    for (j = 0; j < MAX_PRIMES; j++)
        is_prime[j] = 1;

    do {
        for (i = 0, a = 3*prime[n_prime-1]; a < 2*MAX_PRIMES; a += 2*prime[n_prime-1], i++)
            is_prime[a>>1] = 0;
        for (a = 1+(prime[n_prime-1]>>1); a < MAX_PRIMES; a++) {
            if (is_prime[a] == 1) {
                prime[n_prime] = (a<<1)+1;
                n_prime++;
                if (i) break;
            }
        }
    } while (i);
}

static int get_divisors (int n)
{
    int pdt = 1, k, v, t, p;

    t = n;
    for (p = 0; p < n_prime; p++) {
        if (t < prime[p]) break;
        if (t % prime[p]) continue;

        k = v = 1;
        while ((t % prime[p]) == 0) {
            v *= prime[p];
            k += v;
            t /= prime[p];
        }
        pdt *= k;
    }
    if (t > 1) {
        pdt *= (1 + t);
    }
    return (pdt - n);
}

int main (int argc, char *argv[])
{
    int arr[10000];
    int n, s = 0;

    build_prime_table();

    arr[0] = 1;

    for (n = 1; n < 10000; n++) {
        arr[n] = get_divisors(n+1);
    }
    for (n = 0; n < 10000; n++) {
        if (arr[n] != n+1 &&
            arr[n] < 10000 &&
            arr[arr[n]-1] == n+1)
            s += arr[n];
    }
    printf("%d\n", s);

    return 0;
}
