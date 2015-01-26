#include <stdio.h>

static void get_squarings (int n, int pow)
{
    int i, p, c;

    if (n < 1) return;

    for (c = 0, p = 1; p <= n; p <<= 1, c++);
    c--;
    p >>= 1;

    if (pow == 0)
        printf("0, (");
    else
        //printf("x^%d", pow);
        printf("%d, (", pow);

    for (i = 0; i < c-1; i++) {
        //printf(" * (1+x^%d)", (1 << i));
        printf("%d, ", (1 << i));
    }
    if (i == c-1) {
        printf("%d", (1 << i));
    }
    printf(")\n");

    n -= p;
    get_squarings(n, p+pow);
}

int main (int argc, char *argv[])
{
    int n, k;

    scanf("%d %d", &n, &k);
    get_squarings(n, k);

    return 0;
}
