/* Number of solutions to A(4D-A) = N */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N   (1000000)

static int solns[MAX_N];

int main (int argc, char *argv[])
{
    int i, j, d, s;

    for (i = 1; i < MAX_N; i++) {
        for (j = i; j < MAX_N; j += i) {
            d = (i + j/i);
            if (!(d & 3) && i > (d/4)) {
                if (j == 120)
                    printf("[%4d] A = %d, D = %d\n", j, i, d);
                solns[j]++;
            }
        }
    }
    for (i = 1, s = 0; i < MAX_N; i++) {
        if (solns[i] == 10) {
            s++;
        }
    }
    printf("Number of solns = 10 for %d numbers!\n", s);

    return 0;
}
