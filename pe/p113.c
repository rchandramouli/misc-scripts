#include <stdio.h>
#include <math.h>

#include "mytypes.h"

int main (int argc, char *argv[])
{
    u64 inc[10], SI = 0, SD = 0, S, t;
    u64 Si[100];
    int i, j, d = 100;

    for (i = 1; i <= 9; i++)
        inc[i] = 1;

    SI = 9;
    Si[0] = 0;
    Si[1] = 9;

    for (i = 2; i <= d; i++) {
        t = 0;
        for (j = 8; j >= 1; j--) {
            inc[j] += inc[j+1];
            t += inc[j+1];
        }
        t += inc[1];
        Si[i] = t;
        SI += t;
    }

    /* Increasing/Decreasing, bijection & zero inclusion */
    SD = SI;
    for (j = 1, i = d-1; i >= 1; i--, j++) {
        SD += Si[j] * i;
    }

    /* Subtract equal runs from the total sum */
    S = SI + SD - 9*(d);

    printf("Number of increasing sequences < 10^100 = I: %I64d, D: %I64d, S: %I64d\n",
           SI, SD, S);
    return 0;
}
