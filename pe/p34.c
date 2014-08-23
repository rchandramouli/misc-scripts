#include <stdio.h>

static int fact[] = { 
    1, 1, 2, 6, 24, 120, 720, 5040,
    40320, 362880
};

static int is_factorial_sum (unsigned int n)
{
    int t = n, s = 0;

    while (t) {
	s += fact[t % 10];
	t /= 10;
    }
    return (s == n);
}

int main (int argc, char *argv[])
{
    unsigned int s, i;

    for (i = 3, s = 0; i < 10000000; i++) {
	if (is_factorial_sum(i)) {
	    printf("%u\n", i);
	    s += i;
	}
    }
    printf("\n%u\n", s);

    return 0;
}
