#include <stdio.h>

typedef unsigned long long int u64;

static u64 get_digital_sum (u64 n)
{
    u64 s = 0;

    while (n) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

int main (int argc, char *argv[])
{
u64 n, c;

/* Worst of the kind!!! */
for (n = c = 0; n < 1000000000000000000ULL; n += 9) {
if (get_digital_sum(n) == get_digital_sum(137*n))
    c++;
}
printf("\nCounts = %Lu\n", c);

return 0;
}
