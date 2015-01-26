#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[])
{
    int i, j, c, s;

    for (s = 0, i = 2; i < 100; i++) {
        if (i == 10 || i == 100) continue;
        for (j = 1; j < 10000; j++) {
            c = ceil((double)j * log10((double)i));
            if (c == j) {
                s++;
                printf("%d ^ %d has %d digits\n", i, j, c);
            }
        }
    }
    printf("total = %d\n", s);

    return 0;
}
