#include <stdio.h>

int main (int argc, char *argv[])
{
    int n, g[4], x;

    scanf("%d", &n);

    g[0] = g[1] = g[2] = g[3] = 0;

    while (n--) {
        scanf("%d", &x);
        g[x-1]++;
    }

    x = g[3];
    if (g[0] > g[2]) {
        x += g[2];
        g[0] -= g[2];
        g[2] = 0;
    } else {
        x += g[0];
        g[2] -= g[0];
        g[0] = 0;
    }
    x += g[2];

    x += g[1] / 2;
    g[1] &= 1;

    if (g[1] || g[0])
        x += (2*g[1] + g[0] -1)/4 + 1;
    
    printf("%d\n", x);

    return 0;
}
