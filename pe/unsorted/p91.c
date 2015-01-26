#include <stdio.h>

typedef unsigned long long int u64;

static int gcd (int x, int y)
{
    if (x > y) return gcd(y, x);
    if (x == 0) return y;
    return gcd(y % x, x);
}

static inline int min (int x, int y)
{
    if (x < y)
        return x;
    return y;
}

int main (int argc, char *argv[])
{
    u64 tri;
    int i, f;
    int x, y;

    scanf("%d", &f);
    tri = 3*f*f;

    for (x = 1; x <= f; x++) {
        for (y = 1; y <= f; y++) {
            i = gcd(x, y);
            tri += min(y*i/x, (f-x)*i/y)*2;
        }
    }

    printf("%I64d\n", tri);

    return 0;
}
