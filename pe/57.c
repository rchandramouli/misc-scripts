#include <stdio.h>

#include "bnum.h"

int main (int argc, char *argv[])
{
    bnum_t p0, p1, p2;
    bnum_t q0, q1, q2;
    bnum_t *p[3], *q[3];
    int n1, n2, n3, x, pd, qd, c = 0;

    p0.nr = p1.nr = 1;
    p0.digits[0] = 1;
    p1.digits[0] = 3;

    q0.nr = q1.nr = 1;
    q0.digits[0] = 1;
    q1.digits[0] = 2;

    p[0] = &p0; p[1] = &p1; p[2] = &p2;
    q[0] = &q0; q[1] = &q1; q[2] = &q2;

    n1 = 0; n2 = 1; n3 = 2; x = 1;

    while (x < 1000) {
	bnum_add(p[n1], p[n2], p[n3]);
	bnum_add(p[n2], p[n3], p[n3]);
	bnum_add(q[n1], q[n2], q[n3]);
	bnum_add(q[n2], q[n3], q[n3]);

	pd = bnum_count_digits(p[n3]);
	qd = bnum_count_digits(q[n3]);
	if (pd > qd) {
	    printf("--------------------------\n");
	    bnum_dump(p[n3]);
	    bnum_dump(q[n3]);
	    c++;
	}
	    
	n1 = (n1 + 1) % 3;
	n2 = (n2 + 1) % 3;
	n3 = (n3 + 1) % 3;
	x++;
    }

    printf("%d\n", c);

    return 0;
}
