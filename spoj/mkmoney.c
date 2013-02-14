#include <stdio.h>

int main (int argc, char *argv[])
{
	double p, r, s;
	int n, i, c = 0;

	while (1) {
		scanf("%lf %lf %d", &p, &r, &n);
		if (p == 0.0 && r == 0.0 && n == 0) break;
		c++;
		s = p;
		for (i = 0; i < n; i++)
			p += ((int)(p*(r/(n))))/(double)100.0;
		printf("Case %d. $%.2lf at %.2lf%% APR compounded %d times yields $%.2lf\n",
		       c, s, r, n, p);
	}
	return 0;
}

