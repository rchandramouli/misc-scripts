#include <stdio.h>

typedef unsigned long long int u64;

static u64 digit_sum[] = {
	0, 1, 2*2, 3*3*3, 4*4*4*4, 5*5*5*5*5, 6*6*6*6*6*6,
	7*7*7*7*7*7*7, 8*8*8*8*8*8*8*8,	9*9*9*9*9*9*9*9*9
};

static void compute_power_loop (u64 n)
{
	u64 s, c = 0;

	printf("%Lu -> ", n);
	do {
		s = 0;
		while (n) {
			s += digit_sum[n % 10];
			n /= 10;
		}
		c++;
		if ((c % 10) == 0) printf("\n\t");
		printf("%Lu -> ", s);
		n = s;
	} while (n >= 2 && c <= 1000);

	if (c > 1000 && n >= 2) {
		printf("...\n");
	} else {
		printf("%Lu\n", s);
	}
}

int main (int argc, char *argv[])
{
	u64 n;

	while (scanf("%Lu", &n)) {
		compute_power_loop(n);
	}

	return 0;
}
