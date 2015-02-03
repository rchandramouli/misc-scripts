/* Calculate 10^M = k (mod N), find M given k, N */
#include <stdio.h>

int main (int argc, char *argv[])
{
    int M, a, k, N, p, q;

    do {
        scanf("%d %d %d", &a, &k, &N);
        if (N == 0)
            break;

        p = q = 1;
        M = 0;
        do {
            p = (p*10) % N;
            q = (a*p) % N;
            M++;
        } while (q != k && M < N);
        if (q == k) {
            printf("M = %d\n", M);
        } else {
            printf("No M!\n");
        }
        fflush(stdout);
    } while (1);

    return 0;
}
