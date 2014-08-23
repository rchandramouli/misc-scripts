#include <stdio.h>

#define MAX_RANGE   (50*1000000)
#define MAX_PRIMES  (10000)

#define ARRC(x)     (sizeof(x)/sizeof(x[0]))
#define SQ(x)       (prime[x] * prime[x])
#define CU(x)       (SQ(x) * prime[x])
#define P4(x)       (CU(x) * prime[x])

static unsigned long long int prime[MAX_PRIMES];
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

char visits[MAX_RANGE];

int main (int argc, char *argv[])
{
    int i, j, k;
    unsigned long long int V1, c = 0, R;

    build_prime_table();

    //n_prime = 25;
    R = MAX_RANGE;

    for (i = 0; i < n_prime; i++) {
        for (j = 0; j < n_prime; j++) {
            for (k = 0; k < n_prime; k++) {
                V1 = P4(k);
                if (V1 > R)
                    break;

                V1 += CU(j);
                if (V1 > R)
                    break;

                V1 += SQ(i);
                if (V1 > R)
                    break;

                if (!visits[V1]) {
                    c++;
                    visits[V1] = 1;
                }
            }
        }
    }
    printf("counts = %d\n", c);

    return 0;
}
