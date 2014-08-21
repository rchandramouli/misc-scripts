/*
 * Count numbers that satisfy:
 *
 *    2a + b + c + e + 2f + g + i + j + 2k = 0 (mod 3)
 */
#include <stdio.h>

static int wheel[9];

static int roll_number_wheel (void)
{
    int i;

    for (i = 0; i < 9; i++)
        if (wheel[i] != 9)
            break;
    if (i == 9) return 0;

    for (i = 0; i < 9; i++) {
        wheel[i] = (wheel[i] + 1) % 10;
        if (wheel[i])
            break;
    }
    return 1;
}

int main (int argc, char *argv[])
{
    int s;
    unsigned long long int c = 0;

    do {
        s = wheel[0] + wheel[1] + wheel[2] + wheel[3] +
            wheel[4] + wheel[5] + wheel[6] + wheel[7] +
            wheel[8] + (wheel[0] + wheel[4] + wheel[8]);
        if ((s % 3) != 0) continue;

        s /= 3;

    } while (roll_number_wheel());

    printf("%Lu\n", c);

    return 0;
}
