#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MOD_P   (1000000007ULL)

#ifndef MAX_N
#define MAX_N   (1000000)
#endif

/* Till 2^17 */
u64 rep_sq[20];
/* 2^17, (2^17+2^16), etc... */
u64 rep_sum_sq[7];
u64 rep_sum1_sq[20];

#ifdef DEBUG
#define dprintf printf
#else
#define dprintf(...)
#endif

static void make_rep_squares_max_n (u64 v1, char debug)
{
    int i;

    rep_sq[0] = v1;
    for (i = 1; i < 20; i++) 
        rep_sq[i] = (rep_sq[i-1] * rep_sq[i-1]) % MOD_P;

    if (debug) {
        for (i = 0; i < 20; i++) {
            printf("\t2^%d  = %I64d\n", i, rep_sq[i]);
        }
        printf("\n");
    }

    rep_sum_sq[0] = 1;
    rep_sum_sq[1] = rep_sq[19];
    rep_sum_sq[2] = (rep_sq[19] * rep_sq[18]) % MOD_P;
    rep_sum_sq[3] = (rep_sum_sq[2] * rep_sq[17]) % MOD_P;
    rep_sum_sq[4] = (rep_sum_sq[3] * rep_sq[16]) % MOD_P;
    rep_sum_sq[5] = (rep_sum_sq[4] * rep_sq[14]) % MOD_P;
    rep_sum_sq[6] = (rep_sum_sq[5] * rep_sq[9]) % MOD_P;

    if (debug) {
        for (i = 0; i < 7; i++) {
            printf("\trep_sum_sq[%d] = %I64d\n", i, rep_sum_sq[i]);
        }
        printf("\n");
    }

    for (i = 0; i < 20; i++)
        rep_sq[i] = (rep_sq[i] + 1) % MOD_P;

    rep_sum1_sq[0] = rep_sq[0];
    for (i = 1; i < 20; i++)
        rep_sum1_sq[i] = (rep_sum1_sq[i-1] * rep_sq[i]) % MOD_P;

    if (debug) {
        for (i = 0; i < 20; i++) {
            printf("\trep_sum1_sq[%d] = %I64d\n", i, rep_sum1_sq[i]);
        }
        printf("\n");
    }
}

static inline u64 get_mod_n_4 (void)
{
    u64 S = 0;

    S = (S + rep_sum1_sq[1]*rep_sum_sq[0]) % MOD_P;
    S = (S * (rep_sum1_sq[0] - 1)) % MOD_P;

    return S;
}

static inline u64 get_mod_n_10p6 (void)
{
    u64 S = 0, v;

    S = (S + rep_sum1_sq[18] * rep_sum_sq[0]) % MOD_P;
    S = (S + rep_sum1_sq[17] * rep_sum_sq[1]) % MOD_P;
    S = (S + rep_sum1_sq[16] * rep_sum_sq[2]) % MOD_P;
    S = (S + rep_sum1_sq[15] * rep_sum_sq[3]) % MOD_P;
    S = (S + rep_sum1_sq[13] * rep_sum_sq[4]) % MOD_P;
    S = (S + rep_sum1_sq[8] * rep_sum_sq[5]) % MOD_P;
    S = (S + rep_sum1_sq[5] * rep_sum_sq[6]) % MOD_P;
    v = (MOD_P + rep_sum1_sq[0] - 1) % MOD_P;
    S = (S * v) % MOD_P;

    return S;
}

static inline u64 verify_sum_10p6 (u64 v1)
{
    u64 S = 0;
    int i;

    for (i = 1; i <= MAX_N; i++) {
        S = ((1 + S)*v1) % MOD_P;
    }
    return S;
}

int main (int argc, char *argv[])
{
    u64 S, v1;
    s32 i;

    timeit_timer_start();

    for (S = 0, i = 2; i <= MAX_N; i++) {
        v1 = i;
        v1 = (v1 * v1) % MOD_P;
        v1 = (MOD_P - v1 + 1) % MOD_P;

        // S = (S + get_mod_n_4()) % MOD_P;
        if ((i % 100000) == 0) {
            u64 s;

            printf("V = %ld ======================================\n", i);
            make_rep_squares_max_n(v1, 1);
            s = get_mod_n_10p6();
            printf("SUM = %I64d, verified sum = %I64d\n", s, verify_sum_10p6(v1));
            printf("==============================================\n");

            S = (S + s) % MOD_P;
            timeit_timer_peek_and_print();
        } else {
            make_rep_squares_max_n(v1, 0);
            S = (S + get_mod_n_10p6()) % MOD_P;
            //S = (S + get_mod_n_4()) % MOD_P;
        }
    }

    printf("F(%ld) = %I64d\n", i-1, S);
    timeit_timer_peek_and_print();

    return 0;
}
