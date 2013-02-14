#include <stdio.h>

int main (int argc, char *argv[])
{
	int p, q, r, n, d;

	while (1) {
		scanf("%d %d %d", &p, &q, &r);

		if (!p && !q && !r) break;
		if ((p % q) != (r % q)) {
			printf("No accounting tablet\n");
			continue;
		}
		n = 0;
		d = (p-r)/q;
		if (d < 0) d = -d;
		n = (d/3) + (d%3)/2 + (d%3)%2;

		printf("%d\n", n);
	}

	return 0;
}

