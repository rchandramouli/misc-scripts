/* Count number of inversions in a given list of numbers */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

static int *arr, N;

static int count_inversions (int n, int *nlist)
{
    int i, j, inv = 0;

    for (i = 0; i < n-1; i++) {
        for (j = i+1; j < n; j++) {
            if (nlist[i] > nlist[j])
                inv++;
        }
    }

    return inv;
}

int main (int argc, char *argv[])
{
    int i;

    scanf("%d", &N);

    if (N <= 0)
        return 0;

    arr = malloc(sizeof(int) * N);
    if (arr == NULL)
        return -1;

    for (i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }

    printf("%d\n", count_inversions(N, arr));

    return 0;
}
