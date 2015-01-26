/* TBD */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "prime.h"

static u64 intlist[1000000][2];
static u64 nr;

int main_brute_force (int argc, char *argv[])
{
    u64 S = 0;
    u64 g, j, mn = 0, mg = 0;

    S = 1;
    g= 2;
    intlist[2][0] = 2;
    intlist[2][1] = 3;

    do {
        g++;
        for (j = g-1; j >= 0; j--) {
            if (g <= intlist[j][1] && g >= intlist[j][0])
                break;
        }
        if (g-j > mn) {
            mn = g-j;
            mg = g;
        }
        intlist[g][0] = intlist[g-1][1] + 1;
        intlist[g][1] = intlist[g-1][1] + j;
    } while (intlist[g][1] < 1000000000ULL);

    printf("Maximal back-lookup diff = %I64d, g = %I64d, "
           "start = %I64d, end = %I64d\n",
           mn, mg, intlist[mg][0], intlist[mg][1]);

    for (mg = 1, S = 8, mn = 2, j = 2; j < g; j++) {
        if (S >= intlist[j][0] && S <= intlist[j][1]) {
            printf("G(%I64d) = %I64d (diff = %I64d)\n", S, j, j-mg);
            mn++;
            S = mn*mn*mn;
            mg = j;
        }
    }

    return 0;
}


static u64 g[200000], blk[200000], sumg[200000];

int main (int argc, char *argv[])
{
    u32 i;

    g[1] = 1;
    blk[1] = 1;
    sumg[1] = 1;

    for (i = 2; i < 10000; i++) {
        g[i] = 1 + g[i - g[g[i-1]]];
        blk[i] = blk[i-1] + i*g[i];
        sumg[i] = sumg[i-1] + g[i];
    }

    printf("End blocks: g[%u] = %I64d, blk[%u] = %I64d\n",
           i-1, g[i-1], i-1, blk[i-1]);

    while (--i) {
        if (1000000 <= blk[i] && 1000000 > blk[i-1]) {
            printf("Found 10^6 at block = %u (from: %I64d to %I64d) S: %I64d\n",
                   i, blk[i-1], blk[i], sumg[i]);
            printf("G(10^6) = %I64d\n",
                   sumg[i-1] + ((blk[i] - blk[i-1]) / i));
            break;
        }
    }

    for (i = 1; i < 10; i++) {
        printf("S: %I64d, G: %I64d, BLK: %I64d\n", sumg[i], g[i], blk[i]);
    }

    return 0;
}
