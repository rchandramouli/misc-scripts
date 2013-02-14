#include <stdio.h>

int main (int argc, char *argv[])
{
	unsigned long long int n;
	int p;

	scanf("%llu", &n);
	
	p = 1 + (n % 10 == 0);
	printf("%d\n", p);
	if (p & 1)
		printf("%llu\n", n % 10ULL);

	return 0;
}

