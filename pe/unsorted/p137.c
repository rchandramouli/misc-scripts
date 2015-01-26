/* TBD */
/*
 * Enumerate solutions of pell's equation:
 *
 *              M^2 - 5K^2 = -4, M = 5n+1
 *
 * n = 2, gives, 11^2 - 5*5^2 = -4
 */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

struct sset {
    s64 x;
    s64 y;
};

struct sset s1[30], s_1[30], s4[30], s_4[30];

/* Build solutions for X^2 - 5y^2 = 1 */
static void build_soln_s1 (void)
{
    int i;

    s_1[0].x = 2;
    s_1[0].y = 1;
    s1[0].x = 1;
    s1[0].y = 0;

    for (i = 1; i < 20; i++) {
        s_1[i].x = 9*s_1[i-1].x + 20*s_1[i-1].y;
        s_1[i].y = 4*s_1[i-1].x + 9*s_1[i-1].y;

        s1[i].x = 9*s1[i-1].x + 20*s1[i-1].y;
        s1[i].y = 4*s1[i-1].x + 9*s1[i-1].y;
    }
}

static void build_soln_s4 (void)
{
    int i, k;
    u64 vx;

    s4[0].x = 2;
    s4[0].y = 0;

    s_4[0].x = 1;
    s_4[0].y = 1;

    for (i = k = 0; i < 17; i++) {
        vx = s4[0].x*s_1[i].x + 5*s4[0].y*s_1[i].y;
        if ((vx % 5) != 1)
            continue;

        s_4[k].x = vx;
        s_4[k].y = s4[0].x*s_1[i].y + s4[0].y*s_1[i].x;

        printf("I = %d, N = %I64d (-1[%d] => %I64d, %I64d)\n",
               k+1, (s_4[k].x-1)/5, i, s_1[i].x, s_1[i].y);
        k++;
    }
    printf("\n");
    for (i = 0; i < 17; i++) {
        vx = s_4[0].x*s1[i].x + 5*s_4[0].y*s1[i].y;
        if ((vx % 5) != 1)
            continue;

        s_4[k].x = vx;
        s_4[k].y = s_4[0].x*s1[i].y + s_4[0].y*s1[i].x;

        printf("I = %d, N = %I64d (1[%d] => %I64d, %I64d)\n",
               k+1, (s_4[k].x-1)/5, i, s1[i].x, s1[i].y);
        k++;
    }
}

int main (int argc, char *argv[])
{
    build_soln_s1();
    build_soln_s4();
}
