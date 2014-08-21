#include <stdio.h>

int main (int argc, char *argv[])
{
    unsigned int n, m, a;
    unsigned int a1, a2, a3, b1, b2, b3;
    unsigned int v1, v2, v3, v4, v5, c;
    int begin = 0;

    scanf("%u %u %u", &n, &m, &a);

    a1 = ((n-1)/a)+1;
    a3 = (a1 % 10000);
    a1 /= 10000;
    a2 = (a1 % 10000);
    a1 /= 10000;

    b1 = ((m-1)/a)+1;
    b3 = (b1 % 10000);
    b1 /= 10000;
    b2 = (b1 % 10000);
    b1 /= 10000;

    v5 = a3*b3;
    c = v5 / 10000;
    v5 = (v5 % 10000);

    v4 = c + a2*b3 + a3*b2;
    c = v4 / 10000;
    v4 = (v4 % 10000);

    v3 = c + a1*b3 + a2*b2 + b1*a3;
    c = v3 / 10000;
    v3 = (v3 % 10000);

    v2 = c + a2*b1 + a1*b2;
    c = v2 / 10000;
    v2 = (v2 % 10000);

    v1 = c + a1*b1;
    c = v1 / 10000;
    v1 = (v1 % 10000);

    if (c) {
        printf("%u", c);
        begin = 1;
    }
    if (begin) {
        printf("%04u", v1);
    } else if (v1) {
        printf("%u", v1);
        begin = 1;
    }
    if (begin) {
        printf("%04u", v2);
    } else if (v2) {
        printf("%u", v2);
        begin = 1;
    }
    if (begin) {
        printf("%04u", v3);
    } else if (v3) {
        printf("%u", v3);
        begin = 1;
    }
    if (begin) {
        printf("%04u", v4);
    } else if (v4) {
        printf("%u", v4);
        begin = 1;
    }
    if (begin) {
        printf("%04u", v5);
    } else {
        printf("%u", v5);
    }

    printf("\n");

    return 0;
}
