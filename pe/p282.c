/* TBD */
/* Ackermann function
 *
 * A(m, n) = 2[^(m-2)]{n+3} - 3, in Knuth's arrow notation.
 */
#include <stdio.h>

#include "mytypes.h"

#define MOD_14P8  (1475789056ULL)

int main (int argc, char *argv[])
{
    u64 S = 1 + 3 + 7 + 61; /* Till ACK(3, 3) */

    /* Compute ACK4_4, ACK5_5, ACK6_6 (mod MOD_14P8) */

    return 0;
}
