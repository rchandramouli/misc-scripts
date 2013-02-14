/* Note: The additions have no effect. The multiplications have a
 * period over modulo arithmetic. Since we have modulo as 2^x, it is
 * easy to compute the final period which is the required answer.
 */
#include <stdio.h>

int main (int argc, char *argv[])
{
	char icode[10];
	int m, v, n;

	while (1) {
		scanf("%d", &n);
		if (!n) break;

		v = 1 << 16;
		while (n--) {
			scanf("%s %d", icode, &m);

			if (icode[0] == 'A') continue;
			while (!(v & 1) && !(m & 1)) {
				v >>= 1;
				m >>= 1;
			}
		}
		printf("%d\n", v);
	}

	return 0;
}

