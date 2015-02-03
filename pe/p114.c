/* 
 * F(n+1) = 1 + F(n) + F(n-3) + F(n-4) + .. + F(0)
 *
 * F(0) = F(1) = F(2) = 1, F(3) = 2
 */
#include <stdio.h>

#include "mytypes.h"

int main (int argc, char *argv[])
{
    u64 arr[51], S;
    int i, k;

    arr[0] = arr[1] = arr[2] = 1;
    arr[3] = 2;

    for (S = arr[0], k = 0, i = 4; i <= 50; i++) {
        arr[i] = 1 + arr[i-1] + S;
        k++;
        S += arr[k];
    }

    printf("For 7 blocks, count = %I64d\n"
           "For 50 blocks, count = %I64d\n",
           arr[7], arr[50]);

    return 0;
}
