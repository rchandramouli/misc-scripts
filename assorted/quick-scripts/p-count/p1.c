#include <stdio.h>

static int permv[16];
static int N, k, eq;
static unsigned long int ccount;

static void dump_perm (void)
{
    int i;

    for (i = 0; i < N-1; i++)
        printf("%3d, ", permv[i]);
    printf("%3d\n", permv[N-1]);
}

#define ABS(x)                                  \
    ({                                          \
        typeof(x) T = x;                        \
        T = (T > 0) ? T: -T;                    \
        T;                                      \
    })

static int valid_diff_vector (void)
{
    int i, d;

    for (i = 0; i < N; i++) {
        d = ABS(i+1-permv[i]);
        if ((!eq && (d <= k)) ||
            (eq && (d != k)))
            return 0;
    }
    return 1;
}

static void perm (int n)
{
    int i, t;

    if (n == 1) {
        if (valid_diff_vector()) {
            ccount++;
            dump_perm();
        }
        return;
    }
    for (i = 0; i < n; i++) {
        t = permv[n-1];
        permv[n-1] = permv[i];
        permv[i] = t;

        perm(n-1);

        t = permv[n-1];
        permv[n-1] = permv[i];
        permv[i] = t;
    }
}

int main (int argc, char *argv[])
{
    int i;

    scanf("%d %d %d", &N, &k, &eq);

    for (i = 0; i < N; i++)
        permv[i] = i+1;

    ccount = 0;
    perm(N);

    printf("Constraint count = %lu\n", ccount);

    return 0;
}
