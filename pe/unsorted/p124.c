#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"
#include "prime.h"

#define MAX_N 100000
static u32 rad_n[MAX_N+1][2];

static void generate_radicals (void)
{
    int i, j;

    for (i = 1; i <= MAX_N; i++) {
        rad_n[i][0] = 1;
        rad_n[i][1] = i;
    }
    for (i = 2; i <= MAX_N; i++) {
        if (rad_n[i][0] == 1) {
            rad_n[i][0] = i;

            for (j = 2*i; j <= MAX_N; j += i)
                rad_n[j][0] *= i;
        }
    }
}

static inline void swap_rad_elems (int e1, int e2)
{
    int t1, t2;

    t1 = rad_n[e1][0];
    t2 = rad_n[e1][1];
    rad_n[e1][0] = rad_n[e2][0];
    rad_n[e1][1] = rad_n[e2][1];
    rad_n[e2][0] = t1;
    rad_n[e2][1] = t2;
}

static int partition (int from, int to)
{
    int pivot = (from+to)/2;
    int pv, i, j;

    pv = rad_n[pivot][0];
    swap_rad_elems(pivot, from);

    i = from+1;
    j = to;

    for (;;) {
        while (rad_n[i][0] <= pv && i < j)
            i++;
        while (rad_n[j][0] >= pv && j > i)
            j--;
        if (i == j)
            break;
        swap_rad_elems(i, j);
    }
    if (i == to && rad_n[i][0] < rad_n[from][0]) {
        swap_rad_elems(from, to);
        return i;
    } else {
        swap_rad_elems(from, j-1);
        return j-1;
    }
}

static void sort_radicals (int from, int to)
{
    if (from < to) {
        int pivot = partition(from, to);

        sort_radicals(from, pivot-1);
        sort_radicals(pivot+1, to);
    }
}

int main (int argc, char *argv[])
{
    generate_radicals();

    sort_radicals(1, MAX_N);

    printf("%lu\n", rad_n[10000][1]);

    return 0;
}
