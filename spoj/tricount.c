/*
  T(n) = T(n-1) + (2n-1) + n(n-1)/2
  
       = Floor[n(n+2)(2n+1)/8]
*/
#include <stdio.h>

typedef unsigned long long int ull;

int main (int argc, char *argv[])
{
	int t;
	ull n, v;

	scanf("%d", &t);
	while (t--) {
		scanf("%llu", &n);

		v = (n*(n+2)*((n<<1)+1)) >> 3;
		printf("%llu\n", v);
	}
	
	return 0;
}

