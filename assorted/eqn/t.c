#include <stdio.h>

static int get_dsum (int n)
{
    int s = 0;

    while (n) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

int main (int argc, char *argv[])
{
    int i, d1, d2, pd;

    pd = 0;
    for (i = 1; i < 20000; i++) {
        d1 = get_dsum(i);
        d2 = get_dsum(137*i);

        if (d1 == d2) {
            printf("[diff: %4d] %4d ---> %4d (%4d)\n",
                   i - pd, i, 137*i, d1);
            pd = i;
        }
    }

    return 0;
}
