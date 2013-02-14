#include <stdio.h>

int main (int argc, char *argv[])
{
	unsigned long int t, k, n, s = 0;

	scanf("%lu %lu", &t, &k);
	while (t--) {
		scanf("%lu", &n);

		s += !(n % k);
	}
	printf("%lu\n", s);

	return 0;
}

