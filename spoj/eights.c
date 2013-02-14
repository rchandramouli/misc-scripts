#include <stdio.h>

int main (int argc, char *argv[])
{
	unsigned long long int n, x, v;

	scanf("%llu", &n);
	while (n--) {
		scanf("%llu", &x);

		v = 192ULL + (250ULL * --x);
		printf("%llu\n", v);
	}
	
	return 0;
}

