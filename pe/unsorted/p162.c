/*
 * Number of Hexadecimal digits:
 *
 *     F(n+1) = 15*16^n - 43*15^n + 41*14^n - 13*13^n
 */
#include <stdio.h>

#include "mytypes.h"

#define N  (16)

int main (int argc, char *argv[])
{
    u64 n16 = 1, n15 = 1, n14 = 1, n13 = 1, s = 0;
    int i;

    for (i = 0; i < N-1; i++) {
        n16 *= 16ULL;
        n15 *= 15ULL;
        n14 *= 14ULL;
        n13 *= 13ULL;
        s += 15ULL*n16 - 43ULL*n15 + 41ULL*n14 - 13ULL*n13;
    }
    printf("%I64X\n", s);

    return 0;
}

