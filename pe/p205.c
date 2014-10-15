/*
 * PP: (x1, x2, x3, x4, x5, x6, x7, x8, x9), 1 <= xi <= 4 => 4^9
 * CC: (x1, x2, x3, x4, x5, x6), 1 <= xi <= 6 => 6^6
 *
 * A total of 4^9 * 6^6 possibilities. Let (PP, CC) be an entry where
 * S(PP) > S(CC). Probability PP wins CC is given by S(PP, CC)/TOTAL.
 *
 * PP: 9 <= S <= 36
 * CC: 6 <= S <= 36
 */
#include <stdio.h>

typedef unsigned long long int u64;
typedef unsigned long int ul32;

static u64 PP_counts[37];
static u64 CC_counts[37];
static u64 T, S;

#define FOR(x, s, e) for (x = s; x <= e; x++)

static void fill_pp_counts (void)
{
    int x1, x2, x3, x4, x5, x6, x7, x8, x9;

    FOR(x1, 1, 4) FOR(x2, 1, 4) FOR(x3, 1, 4) FOR(x4, 1, 4)
        FOR(x5, 1, 4) FOR(x6, 1, 4) FOR(x7, 1, 4) FOR(x8, 1, 4)
        FOR(x9, 1, 4) {
        PP_counts[x1+x2+x3+x4+x5+x6+x7+x8+x9]++;
    }
}

static void fill_cc_counts (void)
{
    int x1, x2, x3, x4, x5, x6;

    FOR(x1, 1, 6) FOR(x2, 1, 6) FOR(x3, 1, 6)
    FOR(x4, 1, 6) FOR(x5, 1, 6) FOR(x6, 1, 6) {
        CC_counts[x1+x2+x3+x4+x5+x6]++;
    }
}

int main()
{
    int i, j;
    double prob;
    u64 S_P = 0, S_C = 0;

    /* Fill in default counts */
    for (i = 0; i < 9; i++)
        PP_counts[i] = 0;
    for (i = 0; i < 6; i++)
        CC_counts[i] = 0;
    //PP_counts[9] = PP_counts[36] = 1;
    //CC_counts[6] = CC_counts[36] = 1;

    fill_pp_counts();
    fill_cc_counts();

    /* Get arrangement counts */
    for (i = 0; i <= 36; i++) {
        S_P += PP_counts[i];
        S_C += CC_counts[i];
    }

    for (i = 9; i <= 36; i++) {
        printf("%I64d ", PP_counts[i]);
    }
    printf("\n\n");
    for (i = 6; i <= 36; i++) {
        printf("%I64d ", CC_counts[i]);
    }
    printf("\n");

    S = 0;
    for (i = 9; i < 37; i++) {
        for (j = 6; j < i; j++) {
            S += PP_counts[i] * CC_counts[j];
        }
    }
    T = 262144ULL * 46656ULL;

    prob = (double)S / T;

    printf("PP counts = %I64d, CC counts = %I64d\n", S_P, S_C);
    printf("total = %I64d, PP>CC = %I64d (prob = %0.9g)\n", T, S, prob);

    return 0;
}
