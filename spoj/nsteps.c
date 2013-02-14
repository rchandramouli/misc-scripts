#include <stdio.h>

int main (int argc, char *argv[])
{
	long int n, x, y, v;

	scanf("%ld", &n);
	while (n--) {
		scanf("%ld %ld", &x, &y);

		if (x != y && x-y != 2)
			printf("No Number\n");
		else
			printf("%ld\n", ((x >> 1) << 2) + (x & 1) - (x-y));
	}
	return 0;
}

