#include <stdio.h>

int main (int argc, char *argv[])
{
    int m, n, i, x, c, v;
    int j;

    scanf("%d %d", &n, &m);

    c = v = 0;
    for (i = 0; i < n; i++) {
        scanf("%d", &x);
        j = (x + m - 1) / m;
        if (j >= v) {
            c = i + 1;
            v = j;
        }
    }
    printf("%d\n", c);

    return 0;
}
