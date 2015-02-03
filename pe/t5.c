/* ACK test */
#include <stdio.h>

#include "mytypes.h"

#define MOD_14P8  (1475789056ULL)

static u64 ack_steps;

static u64 ack (u32 m, u32 n)
{
    ack_steps++;

    if (m == 0)
        return (n+1);
    if (m == 1)
        return (n+2);
    if (m == 2)
        return (2*n + 3);
    if (m == 3)
        return mod2pn(n);
    if (n == 0)
        return ack(m-1, 1);
    return ack(m-1, ack(m, n-1));
}

int main (int argc, char *argv[])
{
    u32 m, n;
    u64 S;

    while (scanf("%u %u", &m, &n) == 2) {
        ack_steps = 0;
        S = ack(m, n);

        printf("ACK(%u, %u) = %I64u (%I64u steps)\n", m, n, S, ack_steps);
        fflush(stdout);
    }
    return 0;
}
