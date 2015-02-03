#include <stdio.h>
#include <math.h>

#define MAXL   1500000ULL

static int lvect[MAXL+1];
static int hist[256];

typedef unsigned long long int u64;

static u64 gcd (u64 a, u64 b)
{
    if (a > b) return gcd(b, a);
    if (a == 0) return b;

    return gcd((b % a), a);
}

int main (int argc, char *argv[])
{
    u64 m, n, a, b, c, l, lim;
    int i;

    lim = sqrt(MAXL/2);

    for (m = 2; m < lim; m++) {
        for (n = 1; n < m; n++) {
            if (!(((n+m) & 1) && gcd(n, m) == 1))
                continue;
            a = m*m+n*n;
            b = 2*m*n;
            c = m*m-n*n;
            l = a+b+c;

            while (l <= MAXL) {
                lvect[l]++;
                l += a+b+c;
            }
        }
    }
    for (i = 0; i < MAXL; i++) {
        if (lvect[i] >= 256)
            continue;
        hist[lvect[i]]++;
    }
    for (i = 0; i < 256; i++) {
        if (hist[i] == 0)
            continue;
        printf("Length (%d) = %d\n", i, hist[i]);
    }
    return 0;
}
