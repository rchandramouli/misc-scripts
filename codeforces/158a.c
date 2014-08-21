#include <stdio.h>

int main (int argc, char *argv[])
{
    int n, k, v, f, c, i, z;

    scanf("%d %d", &n, &k);

    c = z = 0;
    f = -1;

    for (i = 0; i < n; i++) {
        scanf("%d", &v);

        if (!v && !i)
            break;

        if (i == k-1) {
            f = v;
        }
        if (f >= 0 && v < f)
            break;
        if (!v) z++;
        c++;
    }

    printf("%d\n", c-z);

    return 0;
}
