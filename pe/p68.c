#include <stdio.h>
#include <strings.h>

typedef unsigned long long int u64;

static u64 max_wheel;

static u64 get_wheel (int b[5], int a[5])

{
    int arr[] = {
        b[0], a[0], a[1], b[1], a[1], a[2],
        b[2], a[2], a[3], b[3], a[3], a[4],
        b[4], a[4], a[0]
    };
    int i, j, m;
    u64 sign;

    for (i = 3, j = 0, m = arr[0]; i < 15; i += 3) {
        if (arr[i] < m) {
            j = i;
            m = arr[i];
        }
    }
    sign = 0;
    i = j;
    do {
        sign *= 10;
        if (arr[i] == 10) {
            sign *= 10;
        }
        sign += arr[i];
        sign *= 10;
        if (arr[i+1] == 10) {
            sign *= 10;
        }
        sign += arr[i+1];
        sign *= 10;
        if (arr[i+2] == 10) {
            sign *= 10;
        }
        sign += arr[i+2];

        i = (i + 3) % 15;
    } while (i != j);
    return sign;
}

#define FOR(x, i, f) for(x = (i); x < (f); x++)

static int check_unique (int a[5])
{
    int c[11] = {0}, i;

    for (i = 0; i < 5; i++)
        if (a[i] < 1 || a[i] > 10)
            return 0;
    for (i = 0; i < 5; i++)
        c[a[i]]++;
    for (i = 0; i < 11; i++)
        if (c[i] > 1)
            return 0;
    return 1;
}

int main (int argc, char *argv[])
{
    int a[5], b[5];
    int s, k;
    u64 v;

    FOR(a[0], 1, 10) {
        FOR(a[1], 1, 10) {
            FOR(a[2], 1, 10) {
                FOR(a[3], 1, 10) {
                    FOR(a[4], 1, 10) {
                        if (!check_unique(a)) continue;
                        s = a[0]+a[1]+a[2]+a[3]+a[4];
                        if ((s % 5) != 0) continue;
                        s /= 5;
                        if (s < 3 || s > 7) continue;

                        k = s + 11;
                        b[0] = k - a[0] - a[1];
                        b[1] = k - a[1] - a[2];
                        b[2] = k - a[2] - a[3];
                        b[3] = k - a[3] - a[4];
                        b[4] = k - a[4] - a[0];

                        if (!check_unique(b)) continue;

                        printf("A: ");
                        for (k = 0; k < 5; k++)
                            printf("\t%d ", a[k]);
                        printf("\n");

                        printf("B: ");
                        for (k = 0; k < 5; k++)
                            printf("\t%d ", b[k]);
                        printf("\n");

                        v = get_wheel(b, a);
                        printf("WHEEL:\t%I64d\n\n", v);

                        if (max_wheel < v) {
                            max_wheel = v;
                            printf("%I64d\n", v);
                        }
                    }
                }
            }
        }
    }
    printf("%I64d", max_wheel);

    return 0;
}
