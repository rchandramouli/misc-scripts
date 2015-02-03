#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

int main (int argc, char *argv[])
{
    u64 tile[5];
    int i;

    tile[0] = 0;
    tile[1] = 1;
    tile[2] = 2;
    tile[3] = 4;
    tile[4] = 8;

    i = 4;
    while (i < 50) {
        i++;
        tile[0] = tile[1];
        tile[1] = tile[2];
        tile[2] = tile[3];
        tile[3] = tile[4];
        tile[4] = tile[0] + tile[1] + tile[2] + tile[3];
    };

    printf("%I64d\n", tile[4]);
    return 0;
}
