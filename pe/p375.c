/* TBD */
#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define MOD_PRN   (50515093UL)
#define RND_SEED  (290797UL)

#define MAX_RANGE (2000000000UL)

static char prn[MOD_PRN];
static u32 loc[MOD_PRN];
static u32 ord[MOD_PRN];

int main (int argc, char *argv[])
{
    u32 i, s, linkloc, nr;
    u64 p, v, rep, linkto, sum, M, R;

    timeit_timer_start();

    p = RND_SEED;
    s = 0;

    while (1) {
        v = (p * p) % MOD_PRN;
        s++;
        if (prn[v])
            break;

        prn[v] = 1;
        loc[v] = s;
        p = v;
    }

    printf("Seed: %" PRIu64 ", loop at: %" PRIu64 ", S: %u, @: %u\n",
	   (u64)RND_SEED, v, s, loc[v]);

    linkloc = loc[v];
    linkto = v;
    rep = s;

    for (i = 0, s = 0; s < MOD_PRN; s++) {
        if (prn[s]) {
            ord[i] = s;
            loc[i] = loc[s];
            i++;
        }
    }
    nr = i;

    M = MAX_RANGE / nr;
    R = MAX_RANGE % nr;

    printf("Iter = %" PRIu64 ", value = %" PRIu64 " (link-loc: %u), nr_elems = %u\n",
           rep, linkto, linkloc, nr);
    printf("Smallest = %u (loc: %u), Largest = %u (loc: %u)\n",
           ord[0], loc[0], ord[nr-1], loc[nr-1]);
    printf("Ring count = %" PRIu64 ", remaining = %" PRIu64 "\n",
           MAX_RANGE/nr, MAX_RANGE % nr);

    sum = 0;
    for (i = 0; i < nr; i++) {
        sum += M*ord[i];
        if (loc[i] <= R)
            sum += ord[i];
    }
    printf("First order sum = %"PRIu64"\n", sum);

    timeit_timer_peek_and_print();

    return 0;
}
