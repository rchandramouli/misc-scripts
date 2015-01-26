/* 
 * P114 has:
 * F(n+1) = 1 + F(n) + F(n-3) + F(n-4) + .. + F(0)
 * F(0) = F(1) = F(2) = 1, F(3) = 2
 */
#include <stdio.h>

#include "mytypes.h"

#define MAX_N   (50)

int main (int argc, char *argv[])
{
    u64 arr[MAX_N+2], S;
    int i, j;

    for (i = 0; i < MAX_N; i++)
        arr[i] = 1;
    arr[MAX_N] = 2;

    for (S = arr[0], i = MAX_N+1; ; i++) {
        arr[MAX_N+1] = 1 + arr[MAX_N] + S;

        for (j = 0; j <= MAX_N; j++)
            arr[j] = arr[j+1];

        S += arr[0];

        if (arr[MAX_N+1] >= 1000000)
            break;
    }

    printf("For (%d) blocks, fill exceeds 1M at N=%d, count = %I64d\n",
           MAX_N, i, arr[MAX_N+1]);

    return 0;
}
