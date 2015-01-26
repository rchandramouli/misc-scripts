/* For P383 - factorial */
#include <stdio.h>

#include "mytypes.h"

static u64 get_fact_p5 (u64 n)
{
    u64 s = 0, t = n;

    while (t) {
        s += t / 5;
        t /= 5;
    }
    return s;
}

static int diff_i[20000], diff_p[1000];

u64 main (u64 argc, char *argv[])
{
    u32 n, i, d1, d2, dd1;
    u64 s, p5_0, p5_1;

    scanf("%u", &n);

    d1 = d2 = dd1 = 0;
    for (s = 0, i = 5; i <= n; i += 5) {
        p5_0 = get_fact_p5(i);
        p5_1 = get_fact_p5(2*i-1);
        if (p5_1 < 2*p5_0) {
            d2 = i/5;
            dd1 = 2*p5_0-p5_1;
            printf("\tI = %5u (I/5 = %5u, DELTA = %5u), 2I-1 = %5u "
                   "(p5_0 = %I64u, p5_1 = %I64u, DELTA = %I64u)\n",
                   i, i/5, (d2 - d1), 2*i-1, p5_0, p5_1, dd1);
            s++;
            if (d2-d1 < 20000)
                diff_i[d2-d1] = 1;
            if (dd1 < 1000)
                diff_p[dd1] = 1;
            d1 = d2;
        }
    }

    printf("Diff list I:\n\t");
    for (i = d1 = 0; i < 20000; i++) {
        if (diff_i[i]) {
            d1++;
            printf("%u ", i);
            if ((d1 % 20) == 0)
                printf("\n\t");
        }
    }
    printf("\nDiff list P:\n\t");
    for (i = d1 = 0; i < 1000; i++) {
        if (diff_p[i]) {
            d1++;
            printf("%u ", i);
            if ((d1 % 20) == 0)
                printf("\n\t");
        }
    }
    printf("\n\n");
    printf("T(%u) = %I64u\n", n, s);

    return 0;
}
