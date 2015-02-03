#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[])
{
    int x, y, px = 1, py = 2, a, b;

    for (x = 3; x <= 10000; x += 2) {
        for (y = 4; y <= 10000; y += 2) {
            if ((x*x - 2*y*y) == -7) {
                /* Correlate with previous x/y */
                a = 2*py*y - px*x;
                b = py*x - px*y;
                printf("(%d, %d) --- with (a,b) being (%d/7, %d/7)\n",
                       x, y, a, b);
                px = x;
                py = y;
            }
        }
    }

    px = 1;
    py = 2;

    for (a = 1; a < 10; a++) {
        if (a & 1) {
            x = (11*px + 12*py) / 7;
            y = (6*px + 11*py) / 7;
        } else {
            x = (9*px + 8*py) / 7;
            y = (4*px + 9*py) / 7;
        }
        px = x;
        py = y;

        printf("(a = %d) solution (%d, %d) diff = %d\n",
               a, x, y, (x*x-2*y*y));
    }

    return 0;
}
