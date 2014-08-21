#include <stdio.h>

int main (int argc, char *argv[])
{
    int n, x;
    int c, v[100], t;

    scanf("%d", &n);

    c = t = 0;
    v[t] = 0;

    while (n--) {
        scanf("%d", &x);
        if (x < 0) {
            if (c == 2) {
                t++;
                c = 1;
                v[t] = 1;
            } else {
                c++;
                v[t]++;
            }
        } else {
            v[t]++;
        }
    }

    printf("%d\n", t+1);
    for (c = 0; c <= t; c++)
        printf("%d ", v[c]);

    printf("\n");

    return 0;
}
