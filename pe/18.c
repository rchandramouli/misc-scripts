#include <stdio.h>

static int tri[1024][1024];
static int sum[1024][1024];

#define max(x, y) (((x) > (y)) ? (x): (y))

int main (int argc, char *argv[])
{
    int i, j, k, n;

    n = j = k = 0;
    while (scanf("%d", &i) == 1) {
	tri[k][j] = i;
	if (++j > k) { k++; j = 0; }
    }

    printf("\n%d\n", k);

    sum[0][0] = tri[0][0];
    for (i = 1; i < k; i++) {
	sum[i][0] = sum[i-1][0] + tri[i][0];
	sum[i][i] = sum[i-1][i-1] + tri[i][i];
    }

    for (i = 2; i < k; i++) {
	for (j = 1; j < i; j++) {
	    n = max(sum[i-1][j-1], sum[i-1][j]);
	    sum[i][j] = tri[i][j]+n;
	}
    }

    n = 0;
    printf("\n");
    for (j = 0; j < k; j++) {
	printf("%d ", sum[k-1][j]);
	if (sum[k-1][j] > n)
	    n = sum[k-1][j];
    }

    printf("\n%d\n", n);

    return 0;
}
