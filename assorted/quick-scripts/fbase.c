#include <stdio.h>

#define FOR(v, m)   for(v = 0; v < m; v++)

int main(int argc, char *argv[])
{
	int a0, a1, a2, a3, a4, s;

	a0 = a1 = a2 = a3 = a4 = 0;
	FOR(a4, 5) FOR(a3, 4) FOR(a2, 3) { /* FOR(a1, 2) FOR(a0, 1) { */
		s = 1*a0 + 1*a1 + 2*a2 + 6*a3 + 24*a4;
		printf("%d, ", s);
	}
	printf("\n");

	return 0;
}
