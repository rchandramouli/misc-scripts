#include <stdio.h>

typedef unsigned long long int u64;

#define MAX_DIGITS     20
#define MAX_SUM        9

static u64 n_counts[10][10][MAX_DIGITS]; /* fixing d1, d2, count rest */

static u64 get_number_count (int d1, int d2, int r)
{
    int i;

    if (r == 0)
        return 1;

    if (!n_counts[d1][d2][r]) {
        for (i = 0; i <= MAX_SUM-d1-d2; i++) {
            n_counts[d1][d2][r] += get_number_count(d2, i, r-1);
        }
    }
    return n_counts[d1][d2][r];
}

int main (int argc, char *argv[])
{
    u64 sum = 0;
    int i;

    for (i = 1; i < 10; i++) {
        sum += get_number_count(0, i, MAX_DIGITS-1);
    }
    printf("%Lu\n", sum);

    return 0;
}
