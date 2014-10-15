#include <stdio.h>

typedef long long int i64;

static i64 parts[1000000];

int main (int argc, char *argv[])
{
    i64 v, s;
    int k, i, d, t;
    int mod, max;

    parts[0] = 1;
    parts[1] = 1;
    parts[2] = 2;
    parts[3] = 3;
    parts[4] = 5;

    scanf("%d %d", &mod, &max);

    for (i = 5; ; i++) {
        v = 0;
        for (d = -1; d < 2; d+= 2) {
            for (k = d; ; k += d) {
                t = k*(3*k-1)/2;
                if (t > i) break;
                if (k & 1) {
                    s = v + parts[i-t];
                } else {
                    s = v - parts[i-t];
                }
                if (mod) {
                    v = s % 1000000LL;
                } else {
                    v = s;
                }
            }
        }
        parts[i] = v;
        if (v == 0) {
            printf("p(n) = 0 (mode 1000000), n = %d\n", i);
            break;
        }
        if (max != -1 && i == max) {
            printf("p(%d) = %I64d\n", i, parts[i]);
            break;
        }
    }
    return 0;
}
