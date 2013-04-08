/* Cycle count:
 * for permutation pattens: (1 2 3 4)<->(4 2 1 3), (1 2 3 4 5)<->(5 3 1 2 4)
 */
#include <stdio.h>

int main (int argc, char *argv[])
{
    int v[65537], perm[65537];
    int N, i, j, c, s;

    scanf("%d", &N);
    for (i = 1, j = N, s = -1; i <= N; i++) {
        perm[i] = j;
        v[i] = 0;
        j += s*2;
        if (j <= 0) {
            j = 1-j;
            s = -s;
        }
    }

    i = 1;
    c = 0;
    while (1) {
        i = perm[i];
        if (v[i]) {
            c++;
            for (i = 1; i <= N; i++)
                if (!v[i])
                    break;
            if (i > N)
                break;
        } else {
            v[i] = 1;
        }
    }

    printf("%d %d\n", N, c);

    return 0;
}
