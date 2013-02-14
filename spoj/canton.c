#include <stdio.h>

int main (int argc, char *argv[])
{
	unsigned long long int N, no[2]={1, 1}, tmp=1;
	int tcases;
	short int rem;

	scanf ("%d", &tcases);

	while (tcases--) {
		unsigned long long int nth=1, sum;

		scanf ("%llu", &N);
      
		/* Count on Cantor's diagonalization */
		tmp=N;
		while ((sum=(nth*(nth+1))>>1) < N) nth++;

		sum-=nth--; rem=nth&1; N-=sum;
		no[rem]=nth; no[!rem]=1;

		if (N) { N--; no[rem]=no[rem]+1; }
		no[rem]-=N; no[!rem]+=N;

		printf ("TERM %llu IS %llu/%llu\n", tmp, no[0], no[1]);
	}
	return 0;
}

