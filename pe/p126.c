/* TBD */
/*
 * C(layer, x, y, z)
 *
 * C(1, x, y, z) = 2(xy + yz + zx)
 * C(2, x, y, z) = C(1, x, y, z) + ...
 * C(3, x, y, z) = C(2, x, y, z) + ...
 *
 */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define MAXS   10

int main (int argc, char *argv[])
{
    u32 x, y, n, s, v, count;

    timeit_timer_start();

    n = 1;
    while (1) {
        count = 0;
        s = sqrt(n/3);

        for (x = 1; x <= s; x++) {
            for (y = x; y <= s; y++) {
                v = n - x*y;
                if ((v % (x+y)) == 0)
                    count++;
            }
        }
        if (count >= MAXS) {
            printf("Number of solutions = %lu at N = %lu\n", count, n);
            if (count == MAXS)
            break;
        }
        n += 2;
    }
    timeit_timer_peek_and_print();

    return 0;
}
