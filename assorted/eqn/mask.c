#include <stdio.h>

typedef unsigned long long int u64;
typedef unsigned int u32;

#define PRINT(n, m)                             \
    printf(" / %20Lu  ===>  %08x%08x\n",        \
           n, (u32)(m >> 32), (u32)m)

int main (int argc, char *argv[])
{
    u64 n, m, V;

    V = (u64)-1;

    n = 2;
    while (1) {
        m = V / (n-1);
        PRINT(n-1, m);

        m = V / (n+1);
        PRINT(n+1, m);

        if (m <= 1)
            break;

        n <<= 1;
    }
    return 0;
}
