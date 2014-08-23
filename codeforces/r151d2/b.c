#include <stdio.h>

int main (int argc, char *argv[])
{
    int n, x, s, k, r;

    scanf("%d", &n);

    s = 0;
    k = n;
    while (n--) {
        scanf("%d", &x);
        s += x;
    }

    r = s % k;
    if (r < 0)
        r += k;

    printf("%d\n", (r == 0) ? k: k-1);

    return 0;
}
