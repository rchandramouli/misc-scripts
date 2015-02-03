#include <stdio.h>

#define ONE_MILLION   (1000000)

#ifdef DEBUG
#define dprintf printf
#else
#define dprintf(...)
#endif

static int fchain[2177281];
static int flen[2177281];

static int fv[] = {
    1, 1, 2, 2*3, 2*3*4, 2*3*4*5,
    2*3*4*5*6, 2*3*4*5*6*7,
    2*3*4*5*6*7*8,
    2*3*4*5*6*7*8*9
};

static int fvalue (int n)
{
    int f = 0;

    while (n) {
        f += fv[n % 10];
        n /= 10;
    }
    return f;
}

static int visit_chain (int n, int c)
{
    dprintf("%s: visiting (n=%d, c=%d) flen[%d]=%d\n",
            __func__, n, c, n, flen[n]);

    if (flen[n] == -1) {
        printf("At N = %d, Loop length = %d\n", n, c);
        return (c+1);
    }
    if (flen[n] >= 1) {
        return flen[n];
    }
    if (!flen[n]) {
        flen[n] = -1;
        flen[n] = 1+visit_chain(fchain[n], c+1);
    }
    dprintf("\t%s: returning (n=%d, c=%d) length = %d\n",
            __func__, n, c, flen[n]+1);
    return flen[n];
}

static void dump_chain (int n)
{
    int x[61], t, v;

    printf("\n\n");
    printf("LENGTH of chain @ %d = %d\n", n, flen[n]);
    for (t = 0; t < flen[n]; t++) {
        v = fchain[n];
        x[t] = n;
        n = v;
        printf("%d ", x[t]);
    }
    printf("\n");
}

int main (int argc, char *argv[])
{
    int n, v, pv, c = 0, max;

    for (n = 1; n < ONE_MILLION; n++) {
        if (fchain[n])
            continue;

        v = fvalue(n);
        fchain[n] = v;
        pv = v;

        do {
            v = fvalue(pv);
            if (fchain[pv])
                break;
            fchain[pv] = v;
            pv = v;
        } while (1);
    }

    /* Pre-fill known loop lengths */
    flen[1] = 1;
    flen[2] = 1;
    flen[169] = flen[363601] = flen[1454] = 3;
    flen[871] = flen[45361] = 2;
    flen[872] = flen[45362] = 2;
    flen[145] = 1;
    flen[40585] = 1;

    /* Find all chain lengths */
    for (n = 3; n < ONE_MILLION; n++) {
        if (flen[n])
            continue;

        if (visit_chain(n, 1) == 60) {
            if ((c % 10) == 0) {
                dump_chain(n);
            }
            printf("%s: chain len 60 from %d\n", __func__, n);
            c++;
        }
    }

    printf("\nTotal with chain == 60, C = %d\n", c);

    max = 0;
    for (n = 1; n < ONE_MILLION; n++) {
        if (flen[n] > max)
            max = flen[n];
    }
    printf("Max length = %d\n", max);

    return 0;
}
