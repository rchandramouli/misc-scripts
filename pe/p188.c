/* Tetration */
#include <stdio.h>

#include "mytypes.h"

#define N_EXP    (1777)
#define MOD_N8   (100000000ULL)

/* Compute (n ^ pwr) % mod */
static u64 get_exp_mod (u64 n, u64 mod, u64 pwr)
{
    u64 v = 1, pdt = n;

    while (pwr) {
        if (pwr & 1)
            v = (v * pdt) % mod;
        pdt = (pdt * pdt) % mod;
        pwr >>= 1;
    }
    return v;
}

static u64 mod1_power (u64 n, u64 mod)
{
    int i, j;
    u64 p2, p5, p, v;

    for (p = mod, p2 = 1, i = 0; i <= 8; i++, p2 *= 2) {
        for (p5 = 1, j = 0; j <= 8; j++, p5 *= 5) {
            v = p2*p5;
            if (get_exp_mod(n, mod, v) == 1 && p > v)
                p = v;
        }
    }
    return p;
}

static u64 tetration (u64 n, u64 p, u64 mod)
{
    u64 m;

    if (mod == 4)
        return 1;

    m = mod1_power(n, mod);
    return get_exp_mod(n, mod, tetration(n, p-1, m));
}

int main (int argc, char *argv[])
{
    printf("%I64u\n", tetration(N_EXP, 1855, MOD_N8));

    return 0;
}
