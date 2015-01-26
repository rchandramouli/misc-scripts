#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

static u32 psc[1024];

static void dump_ptable (int r, int maxr)
{
    int i;

    printf("%02d: ", r);
    for (i = 0; i < maxr-r; i++)
        printf(" ");

    for (i = 0; i <= r; i++)
        printf("%c ", (psc[i]) ? '*': ' ');
    printf("\n");
}

#define MAX_R    (49)

#define N_P      (7ULL)
#define NFREE_P  (((N_P+1)/2)*(N_P))

static u64 get_free_count (u32 P, u32 r)
{
    u64 pexp, c, m, v;

    if (r <= P)
        return ((r*(r+1))/2);

    pexp = c = 1;

    while (r > pexp) {
        pexp *= P;
        c *= (((P+1)/2)*P);
    }
    pexp /= P;
    c /= (((P+1)/2)*P);

    printf("R = %lu, pexp = %I64u, c = %I64u\n", r, pexp, c);

    if (r == pexp) {
        return c;
    }

    m = r/pexp;
    return (((m*(m+1))/2)*c + (r/pexp + 1)*get_free_count(P, r-m*pexp));
}

int generate_pascal_main (int argc, char *argv[])
{
    u64 S, T;
    int r, c, u1, u2, rc;

    psc[0] = 1;
    dump_ptable(0, MAX_R);

    psc[1] = 1;
    dump_ptable(1, MAX_R);

    psc[1] = 2;
    psc[2] = 1;
    dump_ptable(2, MAX_R);

    psc[0] = psc[3] = 1; psc[1] = psc[2] = 3;
    dump_ptable(3, MAX_R);

    S = T = 10;

    for (r = 4; r < MAX_R; r++) {
        rc = 1;

        S++;
        T++;
        u1 = psc[0] + psc[1];
        for (c = 1; c < r; c++) {
            T++;
            u2 = psc[c] + psc[c+1];
            psc[c] = u1 % N_P;
            u1 = u2;

            if (psc[c]) {
                S++;
                rc++;
            }
        }
        psc[c] = 1;
        S++;

        dump_ptable(r, MAX_R);
    }
    printf("Number of elems = %I64u (rows = %d)\n", S, r);

    return 0;
}

int main (int argc, char *argv[])
{
    u32 p, r;

    scanf("%lu %lu", &p, &r);
    printf("Number of free elements = %I64u\n", get_free_count(p, r));

    return 0;
}
