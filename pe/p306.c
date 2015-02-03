/* TBD */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N     (1001)
#define PLAYER_A  (1)
#define PLAYER_B  (2)

static char gwin[MAX_N];

static int get_winner (int p, int n)
{
    int a, b;

    if (gwin[n-2] == p)
        return p;

    if (gwin[1] == p && gwin[n-3] == p)
        return p;

    /* Split and check */
    for (a = 2; a <= n/2; a++) {
        b = n-2-a;
        if ((gwin[a] != p && gwin[b] == p) ||
            return p;
    }

    /* 
        if ((gwin[a] != p && gwin[b] == p) ||
            (gwin[a] == p && gwin[b] != p)) {
            
        }
    }
    
}

int main (int argc, char *argv[])
{
    int i, c;

    gwin[1] = PLAYER_B;
    gwin[2] = PLAYER_A;
    gwin[3] = PLAYER_A;
    gwin[4] = PLAYER_A;
    gwin[5] = PLAYER_B;

    for (c = 3, i = 6; i < MAX_N; i++) {
        gwin[i] = get_winner(PLAYER_A, i);
        if (gwin[i] == PLAYER_A)
            c++;
    }
    printf("Games A wins = %d\n", c);

    return 0;
}
