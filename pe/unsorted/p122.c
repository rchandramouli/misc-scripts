#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N    (1024)

static int exp_steps[MAX_N];
static int nline[MAX_N];
static int max_n;

static void fill_exp_steps (int step, int n)
{
    int i, s, f;

    for (i = n; i >= 0; i--) {
        f = 0;
        s = nline[n] + nline[i];
        if (s <= max_n) {
            if (!exp_steps[s] || exp_steps[s] >= step) {
                f = 1;
                exp_steps[s] = step;
            }
        }
        if (f) {
            nline[n+1] = s;
            fill_exp_steps(step+1, n+1);
        }
    }
}

int main (int argc, char *argv[])
{
    int i, S;

    exp_steps[1] = 0;
    exp_steps[2] = 1;

    nline[0] = 1;
    nline[1] = 2;

    max_n = 200;

    fill_exp_steps(2, 1);

    for (i = 1; i <= max_n; i++) {
        S += exp_steps[i];
        printf("%d: %d\n", i, exp_steps[i]);
    }

    printf("\nSum of steps = %d\n", S);

    return 0;
}
