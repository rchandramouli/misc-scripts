#include <stdio.h>

typedef unsigned long long int u64;

static void print_1bit_neigh(u64 k, int n)
{
	int i;
	u64 v;

	for (i = 0; i < n; i++) {
		if (k & (1 << i)) continue;
		v = k | (1 << i);
		printf("n%Lu -- n%Lu;\n", k, v);
	}
}

int main (int argc, char *argv[])
{
	int n;
	u64 i, L;

	scanf("%d", &n);
	if (n <= 0) return -1;

	L = (1 << n);

	printf("graph G {\nnode [label=\"\",fixedsize=true,shape=\"circle\","
	       "style=\"bold,filled\",fontsize=0,color=\"#000000\","
		"width=.1,height=.1];\n"
	       "edge [color=\"#888888\"];\n\n");
	for (i = 0; i < L; i++) {
		print_1bit_neigh(i, n);
	}
	printf("};\n");

	return 0;
}
