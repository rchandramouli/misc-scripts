#include <stdio.h>

int main (int argc, char *argv[])
{
    int n, a1, an, jmp, x, p, dn, c, t;

    scanf("%d", &n);
    scanf("%d", &x);

    t = n;
    jmp = dn = c = 0;
    a1 = an = x;

    while (--n) {
        p = x;
        scanf("%d", &x);

        if (p > x) {
            dn++;
            if (dn > 1) {
                jmp = -1;
                break;
            } else {
                c = n;
                jmp++;
            }
        } else {
            dn = 0;
        }
    }

    if (n <= 0) {
        an = x;
    }

    if (jmp == -1 || jmp > 1) {
        printf("-1\n");
    } else if (jmp == 0) {
        printf("0\n");
    } else {
        if (a1 < an) {
            printf("-1\n");
        } else {
            printf("%d\n", c);
        }
    }

    return 0;
}
