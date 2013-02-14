#include <stdio.h>

int main (int argc, char *argv[])
{
	unsigned long long int n, s, t, p, a;

	scanf("%llu", &t);
	while (t--) {
		scanf("%llu", &n);
		for (s = p = 0; p < n; p++) {
			scanf("%llu", &a);
			s = (s + a) % n;
		}
		printf("%s\n", ((s % n) == 0) ? "YES": "NO");
	}

	return 0;
}

