#include <stdio.h>

typedef unsigned long int ul32;

static char are_digits_odd (ul32 n)
{
    while (n) {
        if (!(n & 1))
            return 0;
        n /= 10;
    }
    return 1;
}

static ul32 reverse_digits (ul32 n)
{
    ul32 r = 0;

    while (n) {
        r = r*10 + (n % 10);
        n /= 10;
    }
    return r;
}

int main (int argc, char *argv[])
{
    ul32 v, n, c = 0;

    scanf("%u", &n);

    for (v = 1; v < n; v++) {
        /* Skip from 10^4 to 10^5 */
        if (v == 10000)
            v = 100000;
        /* Skip from 10^8 to 10^9 */
        if (v == 100000000)
            v = 1000000000;
        if ((v % 10) == 0)
            continue;
        if (are_digits_odd(v + reverse_digits(v)))
            c++;
    }
    printf("%d\n", c);

    return 0;
}
