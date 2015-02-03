/* TBD */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

static int p[6][100];
static int np[6];

#define POLYG_3(n)   (((n)*((n)+1))/2)
#define POLYG_4(n)   ((n)*(n))
#define POLYG_5(n)   (((n)*(3*(n)-1))/2)
#define POLYG_6(n)   ((n)*(2*(n)-1))
#define POLYG_7(n)   (((n)*(5*(n)-3))/2)
#define POLYG_8(n)   ((n)*(3*(n)-2))

#define FILL_POLYG(N)                               \
    for (n = 1, np[N-3] = 0; n < 1000; n++) {       \
        v = POLYG_##N(n);                           \
            if (v >= 1000 && v < 10000) {           \
                p[N-3][np[N-3]++] = v;              \
            }                                       \
    }

/* Find if n1's suffix is prefix of n2 */
static int is_cyclic (int n1, int n2)
{
    if (n1 == n2)
        return 0;

    return (
        /* ((n1 % 1000) == (n2 / 10)) || */
        ((n1 % 100) == (n2 / 100)) ||
        /* ((n1 % 10) == (n2 / 1000)) || */
        0
        );
}

static int cycle[6];
static int visit[6];

#define CYCLE_START  (0)
#define CYCLE_END    (5)

static int find_cyclic_np (int n, int v)
{
    int i, j;

    if (n > CYCLE_END) {
        if (is_cyclic(cycle[CYCLE_END], cycle[CYCLE_START]))
            return 1;
        return 0;
    }
    for (i = CYCLE_START; i <= CYCLE_END; i++) {
        if (visit[i])
            continue;
        visit[i] = 1;

        for (j = 0; j < np[i]; j++) {
            if (is_cyclic(v, p[i][j])) {
                cycle[n] = p[i][j];
                if (find_cyclic_np(n+1, p[i][j]))
                    return 1;
            }
        }
        visit[i] = 0;
    }
    return 0;
}

int main (int argc, char *argv[])
{
    int i, j, k, f, s, n, v;

    timeit_timer_start();

    FILL_POLYG(3);
    FILL_POLYG(4);
    FILL_POLYG(5);
    FILL_POLYG(6);
    FILL_POLYG(7);
    FILL_POLYG(8);

    timeit_timer_peek_and_print();

    printf("NP3 = %d, NP4 = %d, NP5 = %d, NP6 = %d, NP7 = %d, NP8 = %d\n",
           np[0], np[1], np[2], np[3], np[4], np[5]);

    s = 0;
    for (f = 0, i = CYCLE_START; !f && i <= CYCLE_END; i++) {
        visit[i] = 1;
        for (j = 0; j < np[i]; j++) {
            cycle[0] = p[i][j];
            if (find_cyclic_np(1, p[i][j])) {
                f = 1;
                printf("Found cycle: ");
                for (k = CYCLE_START; k < CYCLE_END; k++) {
                    s += cycle[k];
                    printf("%d -- ", cycle[k]);
                }
                s += cycle[CYCLE_END];
                printf("%d\n", cycle[CYCLE_END]);
                printf("Sum = %d\n", s);
                break;
            }
        }
    }
    if (!f) {
        printf("No cycle found!\n");
    }
    timeit_timer_peek_and_print();

    return 0;
}
