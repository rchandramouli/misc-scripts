#include <stdio.h>

static inline int get_dth_value(int N, int n, int *a)
{
	int i;

	n++;
	for (i = 0; i < N; i++) {
		if (a[i]) {
			n--;
			if (!n) {
				n = a[i];
				a[i] = 0;
				return n;
			}
		}
	}

	return 0;
}


int main(int argc, char *argv[])
{
	int div, N, perms[11], i, d, l;
	int init[11];

	scanf("%d", &N);

	for (i = 0; i < 10; i++)
		init[i] = i+1;

	div = 9*8*7*6*5*4*3*2*1;
	for (i = 0, l = 9; l; i++, l--) {
		d = N/div;

		perms[i] = get_dth_value(10, d, init)-1;

		N = (N % div);
		if (l) div /= l;
	}
	perms[9] = get_dth_value(10, 0, init)-1;

	for (i = 0; i < 10; i++)
		printf("%d ", perms[i]);
	printf("\n");

	return 0;
}
