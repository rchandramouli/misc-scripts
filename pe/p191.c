/*
 * f(0) = 1, f(1) = 3, f(2) = 8
 * g(0) = 1, g(1) = 2, g(2) = 4
 *
 *   g(n) = g(n-1) + g(n-2) + g(n-3)
 *   f(n) = g(n) + f(n-1) + f(n-2) + f(n-3)
 */
#include <stdio.h>

typedef unsigned long long int u64;

int main (int argc, char *argv[])
{
    u64 f[64], g[64];
    int i;

    f[0] = 1;
    f[1] = 3;
    f[2] = 8;

    g[0] = 1;
    g[1] = 2;
    g[2] = 4;

    for (i = 3; i < 31; i++) {
        g[i] = g[i-1] + g[i-2] + g[i-3];
        f[i] = g[i] + f[i-1] + f[i-2] + f[i-3];
    }
    printf("%Lu\n", f[i-1]);

    return 0;
}
