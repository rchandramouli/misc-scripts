#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long int ull;

ull reverse (ull);

int main (void)
{
	ull tcase, a, b, t, c, v;
	int carry, p, q;

	scanf("%llu", &tcase);
	while (tcase--) {
		scanf("%llu %llu", &a, &b);

		c = 1;

		t = a; p = 0;
		while (a) {
			a /= 10; p++;
			c *= 10;
		}
		a = t; t = b; q = 0;
		while (b) {
			b /= 10; q++;
		}
		b = t;
		while (p > q) {
			b *= 10;
			q++;
		}
		while (q > p) {
			a *= 10;
			c *= 10;
			p++;
		}
		c /= 10; t = carry = 0;
		while (c) {
			v = (a/c) + (b/c) + carry;
			carry = v/10;
			t = 10*t + (v%10);

			a %= c; b %= c; 
			c/=10;
		}
		if (carry) {
			t = t*10 + carry;
		}
		printf("%llu\n", t);
	}

	return(0);
}

