#include <stdio.h>

typedef unsigned long long int u64;

static u64 euclid_steps (u64 x, u64 y)
{
    u64 c, t;

    if (y == 0)
        return 1;

    c = 0;
    while (y) {
        c++;
        t = y;
        y = x % y;
        x = t;
    }
    return (c);
}

int main (int argc, char *argv[])
{
    u64 i, j, s, v;
    u64 max_n = 1000000;

    //u64 m, n;
    //scanf("%Lu %Lu", &m, &n);
    //printf("%Lu\n", euclid_steps(m, n));
    //return 0;

    s = 0;
    for (i = 1; i <= max_n; i++) {
        s += max_n / i;
    }

    printf("%Lu\n", s);
    return 0;
}
