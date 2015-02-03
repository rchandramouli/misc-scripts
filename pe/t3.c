/* Move odd numbers < 15 to bins such that some multiple of these
 * odds-3 is divisible by 8
 */
#include <stdio.h>

int main (int argc, char *argv[])
{
    int i, j, v;

    for (i = 1; i < 16; i += 2) {
        for (j = 1; ; j+= 2) {
            v = (j*i) & 0xF;
            if (v == 3 || v == 11) { /* 0011 or 1011 */
                break;
            }
        }
        printf("I = %d, J = %d\n", i, j);
    }

    return 0;
}
