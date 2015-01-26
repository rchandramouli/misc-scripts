/* TBD */
/* Fractran machine */

#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

struct tupleblock {
    u32 ppwr[10];
};

static struct tupleblock tp_p[14];
static struct tupleblock tp_q[14];
static struct tupleblock iterblock;

static int pmap[30] = {
    [2] = 0, [3] = 1, [5] = 2, [7] = 3,
    [11] = 4, [13] = 5, [17] = 6, [19] = 7,
    [23] = 8, [29] = 9
};

static int fr_p[] = { 17, 78, 19, 23, 29, 77, 95, 77, 1, 11, 13, 15, 1, 55 };
static int fr_q[] = { 91, 85, 51, 38, 33, 29, 23, 19, 17, 13, 11, 2, 7, 1 };

static u64 plist[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 };

static void decompose (int n, struct tupleblock *t)
{
    int i;

    i = 2;
    while (n > 1) {
        if ((n % i) == 0) {
            t->ppwr[pmap[i]]++;
            n /= i;
        } else {
            i++;
        }
    }
}

/* Check if d divides n */
static inline int block_divides (struct tupleblock *n, struct tupleblock *d)
{
    int r;

    for (r = 0; r < 10; r++) {
        if (n->ppwr[r] < d->ppwr[r])
            return 0;
    }
    return 1;
}

static u64 mod_tuple_block (struct tupleblock *t, u64 p)
{
    int r, k;
    u64 v = 0;

    for (r = 0; r < 10; r++) {
        if (t->ppwr[r]) {
            v = 1;
            break;
        }
    }
    for (; r < 10; r++) {
        for (k = 0; k < t->ppwr[r]; k++)
            v = (v * plist[r]) % p;
    }
    return v;
}

static int multiply_tuple_block (struct tupleblock *t)
{
    int r, i;

    for (r = 0; r < sizeof(fr_p)/sizeof(int); r++) {
        if (block_divides(t, &tp_q[r])) {
            for (i = 0; i < 10; i++) {
                t->ppwr[i] -= tp_q[r].ppwr[i];
                t->ppwr[i] += tp_p[r].ppwr[i];
            }
            return r;
        }
    }
}

static int power2 (struct tupleblock *t)
{
    int r;

    for (r = 1; r < 10; r++)
        if (t->ppwr[r])
            return 0;
    return 1;
}

static void dump_tupleblock (struct tupleblock *t, u64 iter, u64 mod, int idx)
{
    int r;

    printf("ITER: %4I64u, mod: %2I64u, idx: %2d -- (%lu",
           iter, mod, idx, t->ppwr[0]);
    for (r = 1; r < 10; r++) {
        printf(", %lu", t->ppwr[r]);
    }
    printf(")\n");
}

int main (int argc, char *argv[])
{
    u64 seed, iter, piter, mod;
    int i, j, pc;

    timeit_timer_start();

    /* Decompose all into prime units */
    for (i = 0; i < sizeof(fr_p)/sizeof(int); i++)
        decompose(fr_p[i], &tp_p[i]);
    for (i = 0; i < sizeof(fr_q)/sizeof(int); i++)
        decompose(fr_q[i], &tp_q[i]);

    decompose(2, &iterblock);
    piter = 0;
    for (pc = 0, iter = 1; ; iter++) {
        j = multiply_tuple_block(&iterblock);
        mod = mod_tuple_block(&iterblock, plist[pc]);

        //dump_tupleblock(&iterblock, iter, mod, j+1);
        printf("%2d, ", j+1);

        if (iter == 10000)
            break;
        if (power2(&iterblock)) {
            pc++;
            if (pc == 10001)
                break;
            printf("\n\t(%5d) %2I64u at iterations = %I64u "
                   "(delta: %I64u, mod = %I64u)\n",
                   pc, iterblock.ppwr[0], iter, (iter-piter), mod);
            fflush(stdout);
            piter = iter;
        }
    }

    timeit_timer_peek_and_print();
    printf("Number of iterations to produce 2^(%lu) = %I64u\n",
           iterblock.ppwr[0], iter);

    return 0;
}
