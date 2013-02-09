#include <stdio.h>

static int gcd (int a, int b)
{
    if (a == 0) return b;
    if (a > b) return gcd(b, a);

    return gcd(b%a, a);
}

int main (int argc, char *argv[])
{
    int p, q;
    int p1, q1, p2, q2, g1, g2;

    for (q = 20; q < 100; q++) {
	for (p = 10; p < q; p++) {
	    g1 = gcd(p, q);
	    if (g1 == 1) continue;

	    p1 = p/g1; q1 = q/g1;
	    p2 = p/10; q2 = q % 10; g2 = gcd(p2, q2);
	    p2 /= g2; q2 /= g2;

	    if (p1 == p2 && q1 == q2 && ((p%10) == (q/10))) {
		printf("%d / %d \n", p, q);
	    }
	}
    }

    return 0;
}
