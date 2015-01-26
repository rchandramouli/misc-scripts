#include <stdio.h>

typedef unsigned long long int u64;

static inline u64 fn (int n)
{
    u64 v1, v2, v3;

    v1 = 1 + n*n;
    v2 = (v1 - 1)*(v1 - 1) + 1;
    v3 = (v2 - 1)*(v2 - 1);

    return (v3*(v1-(u64)n) - (n-1)*v1*v2);
}

int main (int argc, char *argv[])
{
    int n;
    u64 v;

    for (n = 1; n < 11; n++) {
	v = fn(n);
	printf("n = %d, f(n) = %Lu\n", n, v);
    }

    return 0;
}
