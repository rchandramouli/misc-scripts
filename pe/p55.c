#include <stdio.h>

typedef unsigned long long int u64;

static u64 get_reverse (u64 n)
{
    u64 r = 0;

    while (n) {
        r = r*10 + (n % 10);
        n /= 10;
    }
    return r;
}

static int is_palindrome (u64 n)
{
    return (n == get_reverse(n));
}

static int is_lychrel (u64 n)
{
    u64 t;
    int iter = 51;

    while (iter--) {
        t = get_reverse(n) + n;
        if (is_palindrome(t))
            return 0;
        n = t;
    }
    return 1;
}

int main (int argc, char *argv[])
{
    int n, c, i;

    scanf("%d", &n);

    for (i = 1, c = 0; i <= n; i++) {
        if (is_lychrel(i)) {
            printf("%d\n", i);
            c++;
        }
    }
    printf("%d\n", c);

    return 0;
}
