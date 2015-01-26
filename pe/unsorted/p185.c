/* TBD */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

struct guessN_s {
    u64 N;
    int ncorrect;
    u64 g0;
    u64 g1;
};

static struct guessN_s vect1[] = {
    {2321386104303845, 0, 1111111111111111},

    {3847439647293047, 1, 1111111111111111},
    {3174248439465858, 1, 1111111111111111},
    {8157356344118483, 1, 1111111111111111},
    {6375711915077050, 1, 1111111111111111},
    {6913859173121360, 1, 1111111111111111},
    {4895722652190306, 1, 1111111111111111},

    {5616185650518293, 2, 1111111111111111},
    {4513559094146117, 2, 1111111111111111},
    {2615250744386899, 2, 1111111111111111},
    {6442889055042768, 2, 1111111111111111},
    {2326509471271448, 2, 1111111111111111},
    {5251583379644322, 2, 1111111111111111},
    {2659862637316867, 2, 1111111111111111},

    {5855462940810587, 3, 1111111111111111},
    {9742855507068353, 3, 1111111111111111},
    {4296849643607543, 3, 1111111111111111},
    {7890971548908067, 3, 1111111111111111},
    {8690095851526254, 3, 1111111111111111},
    {1748270476758276, 3, 1111111111111111},
    {3041631117224635, 3, 1111111111111111},
    {1841236454324589, 3, 1111111111111111},
};
static int sz = sizeof(vect1) / sizeof(vect1[0]);

static u64 pos10[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
    10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000,
    1000000000000000, 10000000000000000,
};

static inline int get_pos_digit (u64 N, int pos)
{
    return ((N % pos10[pos+1]) / pos10[pos]);
}

static inline u64 pos_flag_unselect (u64 N, int pos)
{
    return ((get_pos_digit(N, pos) == 0) ? N: (N - pos10[pos]));
}

static inline u64 pos_flag_select (u64 N, int pos)
{
    return ((get_pos_digit(N, pos) == 1) ? N: (N + pos10[pos]));
}

static void update_guesses (u64 no_correct, int idx)
{
    int i, d;

    for (i = 0; i < sz; i++) {
        if (i == idx)
            continue;

        for (d = 0; d < 16; d++) {
            if (get_pos_digit(no_correct, d) == get_pos_digit(vect1[i].N, d))
                vect1[i].g0 = pos_flag_clear(vect1[i].g0, d);
        }
    }
}

static void pick_no_correct_guesses (void)
{
    int i;

    for (i = 0; i < sz; i++) {
        if (vect1[i].ncorrect == 0) {
            vect1[i].g0 = 0;
            update_guesses(vect1[i].N, i);
        }
    }
}

static void dump_table (void)
{
    int i;

    for (i = 0; i < sz; i++) {
        printf("%I64d   %d   %016I64d\n",
               vect1[i].N, vect1[i].ncorrect, vect1[i].g0);
    }
}

int main (int argc, char *argv[])
{
    u64 N;
    int i;

    pick_no_correct_guesses();
    dump_table();

    /* Pick the first row that has 
    

    return 0;
}

