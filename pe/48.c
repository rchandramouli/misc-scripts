#include <stdio.h>

#define POWER10_MOD   (10000000000ULL)

int main (int argc, char *argv[])
{
	unsigned long long int sum, i, j, pdt;

	for (sum = 0, i = 1; i < 1000; i++) {
		if ((i % 10) == 0)
			continue;
		pdt = i;
		for (j = 2; j <= i; j++) {
			pdt = (pdt * i) % POWER10_MOD;
		}
		sum += pdt;
	}
	printf("%Lu\n", sum % POWER10_MOD);
	return 0;
}
