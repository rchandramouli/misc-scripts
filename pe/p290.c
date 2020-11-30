#include <stdio.h>

typedef unsigned long long int u64;

static u64 get_digital_sum (u64 n)
{
    u64 s = 0;

    while (n) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

int main (int argc, char *argv[])
{
    u64 n, c;

    /* Worst of the kind!!! */
    //for (n = c = 0; n < 1000000000000000000ULL/137ULL; n += 9) {
    for (n = c = 0; n < 100000ULL; n+=9) {
	if (get_digital_sum(n) == get_digital_sum(137*n)) {
	    c++;
	    printf("----> %Lu->%Lu (x %Lu->%Lu) -- (%Lu)\n",
		   n, n/9, 137*n, 137*n/9, get_digital_sum(n));
	}
    }
    printf("\nCounts = %Lu\n", c);

    return 0;
}

/* 137*n digit sum seq:
 * 0,  11, 13,  6, 17, 19, 12, 23, 16, 9,
 *     11, 13, 15, 17, 19, 12, 14, 16, 18,
 *     11, 13, 24,  8, 10, 21, 14, 16, 27
 */
