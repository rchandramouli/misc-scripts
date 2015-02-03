#include <stdio.h>
#include <math.h>

static int is_prime (int n)
{
    int sq = sqrt((double)n);
    int k;

    if (n == 2) return 1;
    if (!(n & 1)) return 0;

    for (k = 3; k <= sq; k += 2) {
        if ((n % k) == 0)
            return 0;
    }
    return 1;
}

int main (int argc, char *argv[])
{
    int i;
    int m1 = 0, p1 = 0, mp1 = 0;

    for (i = 1; i < 1000; i++) {
        if (!m1 && !is_prime(6*i-1)) {
            m1 = 1;
            printf("6i-1 = %d, i = %d\n", 6*i-1, i);
        }
        if (!p1 && !is_prime(6*i+1)) {
            p1 = 1;
            printf("6i+1 = %d, i = %d\n", 6*i+1, i);
        }
        if (!mp1 && !is_prime(6*i+1) && !is_prime(6*i-1)) {
            mp1 = 1;
            printf("6i-1 = %d, 6i+1 = %d, i = %d\n",
                   6*i-1, 6*i+1, i);
        }
        if (m1 && p1 && mp1)
            break;
    }

    return 0;
}
