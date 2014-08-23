/* Trivial brute-force */
#include <stdio.h>

static int is_right_triangle (int a, int b, int c)
{
    return ((a*a+b*b) == (c*c));
}

int main (int argc, char *argv[])
{
    int x, y, z, s, i, s_i;
    int solns[2000];

    for (i = 0; i < 2000; i++)
	solns[i] = 0;

    for (x = 1; x < 2000; x++) {
	for (y = x+1; y < 2000; y++) {
	    for (z = y+1; z < 2000; z++) {
		if (x + y + z > 1000)
		    break;
		    
		if (is_right_triangle(x, y, z)) {
		    printf("(%4d, %4d, %4d)\n", x, y, z);
		    solns[x+y+z]++;
		}
	    }
	}
    }

    for (i = s = 0; i <= 1000; i++) {
	if (solns[i] > s) {
	    s = solns[i];
	    s_i = i;
	}
    }
    printf("%d: %d\n", s_i, s);

    return 0;
}
