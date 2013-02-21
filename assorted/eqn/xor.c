#include <stdio.h>

typedef unsigned long long int u64;
typedef unsigned char u8;

union uconv {
	u64 N;
	u8 byte[8];
};

static char precomputed_ones[256];

static void do_precompute_ones (void)
{
	int i, j, c;

	for (i = 1; i < 256; i++) {
		j = i;
		c = 0;
		while (j) {
			c++;
			j &= j-1;
		}
		precomputed_ones[i] = c;
	}
}

static int count_ones (u64 n)
{
	union uconv u;
	int i, c;

	u.N = n;
	for (i = c = 0; i < 8; i++) {
		c += precomputed_ones[u.byte[i]];
	}

	return c;
}

static void dump_xor (int m)
{
	u64 n;
	int t;

	for (n = t = 0; n < 100; n++, t=(t+1)%8) {
		if (!t) {
			printf("\n%8Lu: ", n);
		}
		printf("%3d ", count_ones(n ^ (m*n)));
	}
	printf("\n");
}

int main (int argc, char *argv[])
{
	do_precompute_ones();
	dump_xor(3);

	return 0;
}

