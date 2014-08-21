#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[])
{
    unsigned long long int l, b, h;
    unsigned long long int x1, x2, x3, x, t;
    unsigned long long int c = 0, n = 10000;

    for (l = 1; l <= n; l++) {
        for (b = 1; b <= l; b++) {
            for (h = b; h <= l; h++) {
                x = x1 = l*l + (b+h)*(b+h);
                x2 = b*b + (l+h)*(l+h);
                x3 = h*h + (l+b)*(l+b);

                if (x > x2)
                    x = x2;
                if (x > x3)
                    x = x3;

                t = sqrtl((long double)x);
                if (x == t*t) {
                    // printf("L = %4d, B = %4d, H = %4d (x = %4d, l = %4d)\n",
                    // l, b, h, x, t);
                    c++;
                }
                if (c >= 1000000) {
                    printf("L = %Lu, B = %Lu, H = %Lu (c = %Lu)\n",
                           l, b, h, c);
                    return 0;
                }
            }
        }
        if (l == 1817) {
            printf("At n = %Lu, c = %Lu\n", n, c);
        }
    }
    printf("count = %d\n", c);
    return 0;
}
