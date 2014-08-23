#include <stdio.h>
#include <math.h>

static inline int is_p5 (int n)
{
    int t = sqrt(1+24*n), v = 1+24*n;

    return ((t*t == v) && (((1+t) % 6) == 0));
}

static inline int p5 (int n) { return (n*(3*n-1)/2); }

int main (int argc, char *argv[])
{
    int k, j;
    int pk, pj, pp, pq;
    unsigned int d = -1;

    for (k = 1; k < 10000; k++) {
	pk = p5(k);
	for (j = k+1; j < 10000; j++) {
	    pj = p5(j);

	    pp = pk + pj;
	    pq = pj - pk;

	    if (is_p5(pp) && is_p5(pq)) {
		printf("(j: %d, k: %d) %d %d %d %d\n",
		       j, k, pk, pj, pp, pq);
		if (d > pq) {
		    d = pq;
		}
	    }
	}
    }
    printf("\n%d\n", d);

    return 0;
}
