/* TBD */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MAX_N  (10000)

static int list[MAX_N+1], nr_list = MAX_N;
static u64 mult[MAX_N+1];
static int ncount;

#define MAX_WHEEL   (22)
static char odo012[MAX_WHEEL];

static void roll_next_number (void)
{
    int r;

    for (r = 0; r < MAX_WHEEL; r++) {
        if (odo012[r] == 2)
            odo012[r] = 0;
        else {
            odo012[r]++;
            break;
        }
    }
}

static u64 get_next_number (void)
{
    u64 p = 0;
    int r;

    roll_next_number();
    for (r = MAX_WHEEL-1; r >= 0; r--) {
        p = (p*10) + odo012[r];
    }
    return p;
}

static void record_multiple (u64 n)
{
    int r;

    for (r = 2; r <= nr_list; r++) {
        if (mult[list[r]])
            continue;
        if ((n % list[r]) == 0) {
            mult[list[r]] = n / list[r];
            ncount++;
            list[r] = list[nr_list];
            nr_list--;
            r--;
        }
    }
}

int main (int argc, char *argv[])
{
    u64 s;
    int r;

    timeit_timer_start();

    odo012[0] = 1;
    odo012[1] = 0;

    for (r = 2; r <= MAX_N; r++)
        list[r] = r;
    nr_list = MAX_N;

    /* Count all multiples except for n = 9999 */
    ncount = 1;
    while (ncount < MAX_N-1) {
        record_multiple(get_next_number());
    }

    s = 1;
    for (r = 2; r <= MAX_N; r++)
        s += mult[r];

    /* For repunit, the number is (1)[*n](2)[*4n] */
    s += (11112222222222222222ULL / 9999ULL);

    timeit_timer_peek_and_print();

    printf("Sum of all factors = %I64u\n", s);

    return 0;
}
