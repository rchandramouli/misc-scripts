#include <stdio.h>

#define MAXSIZE   10005

int main (int argc, char *argv[])
{
	int n, s, ct[MAXSIZE], t, c;

	while (1) {
		scanf("%d", &n);

		if (n == -1) break;
		for (s = t = 0; t < n; t++) {
			scanf("%d", &ct[t]);
			s += ct[t];
		}
		if ((s % n) != 0) {
			printf("-1\n");
			continue;
		}
		s /= n;
		for (c = t = 0; t < n; t++)
			c += (ct[t] < s) ? s-ct[t]: 0;
		printf("%d\n", c);
	}
	return 0;
}

