#include <stdio.h>

#include "mytypes.h"
#include "prime.h"

int main (int argc, char *argv[])
{
    int n, i;
    u64 S;

    generate_primes();

    while (1) {
        printf("> ");
        fflush(stdout);
        scanf("%d", &n);
        for (i = 0, S = 1; i < n; i++)
            S *= 10;

        for (i = 3; i < nr_primes && ptable[i] < S; i++) {
            if ((S % ptable[i]) == 1)
                printf("%d ", ptable[i]);
        }
        printf("\n\n");
    }
    return 0;
}
