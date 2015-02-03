/* TBD */
#include <stdio.h>

#include "mytypes.h"

static int perm[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static void remove_elem (int idx)
{
    int i;

    for (i = idx; i < 9; i++) {
        perm[i] = perm[i+1];
    }
}

int main (int argc, char *argv[])
{
    u32 rank = 1000000-1;
    u32 f, k, d;

    f = 10*9*8*7*6*5*4*3*2*1;

    for (k = 1; k <= 10; k++) {
        f = f/(10-k+1);
        d = rank / f;
        printf("%d ", perm[d]);
        remove_elem(d);

        rank = rank % f;
    }

    return 0;
}
