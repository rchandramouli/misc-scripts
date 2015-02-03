#include <stdio.h>

typedef unsigned long long int u64;

#define MAX_SIZE 100
#define MAX_PDT  1000000000ULL

static int ptable[MAX_SIZE], pflag[MAX_SIZE];
static int odo[MAX_SIZE];
static int nr_primes;

static void generate_primes (void)
{
    int i, j;

    ptable[nr_primes++] = 2;

    for (i = 3; i < MAX_SIZE; i += 2) {
        if (pflag[i])
            continue;
        ptable[nr_primes++] = i;
        for (j = 2*i; j < MAX_SIZE; j += i)
            pflag[j] = 1;
    }
}

int main (int argc, char *argv[])
{
    u64 P = 1, s = 1, c = 1, max = MAX_PDT;
    int i, j, k = 0;

    generate_primes();

    //nr_primes = 3;
    //max = 15;
    //max = 100000000ULL;

    while (k < nr_primes) {
        odo[k]++;
        P *= ptable[k];

        if (P > max) {
            /* Find the first non-zero wheel, zero it and then
             * increase the next one by 1.
             */
            for (i = 0; !odo[i] && i < nr_primes; i++);
            if (i == nr_primes)
                break;

            for (j = 0; j < odo[i]; j++)
                P /= ptable[i];
            odo[i] = 0;
            k = i+1;
        } else {
            //printf("%I64d\n", P);
            s += P;
            c++;
            k = 0;
        }
    }
    printf("Sum = %I64d, Count = %I64d\n", s, c);

    return 0;
}
