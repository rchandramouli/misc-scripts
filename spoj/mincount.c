#include <stdio.h>

/* Implementing the formulae would overflow! */
typedef unsigned long long int ull;

int main (int argc, char *argv[])
{
	int n, r;
	ull h, s;

	scanf("%d", &n);
	while (n--) {
		scanf("%llu", &h);
		s = 0;
		if (h < 2) {
			printf("%llu\n", 0ULL);
		} else {
			h--;
			r = h % 6;
			if (r % 3) {
				s = ((r+1)*(r+2))/6;
			} else {
				s = (r*(r+3))/6;
			}
			h = (h - r)/6;
			s += 6*h*h + h*(2*r+3) + ((r*(r+3))>>6);
			printf("%llu\n", s);
		}
	}
	
	return 0;
}

