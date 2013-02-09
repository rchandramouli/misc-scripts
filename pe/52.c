#include <stdio.h>
#include <string.h>

typedef unsigned long long int u64;

static int freq_x[10], freq_nx[10];

static int is_freq_x (u64 n, int k)
{
    n *= k;
    memset(freq_nx, 0, sizeof(freq_nx));
    while (n) {
	freq_nx[n % 10]++;
	n /= 10;
    }
    return !memcmp(freq_x, freq_nx, sizeof(freq_nx));
}

static int is_special_x (u64 n)
{
    u64 temp;

    memset(freq_x, 0, sizeof(freq_x));

    temp = n;
    while (temp) {
	freq_x[temp % 10]++;
	temp /= 10;
    }

    return
	(is_freq_x(n, 2) &&
	 is_freq_x(n, 3) &&
	 is_freq_x(n, 4) &&
	 is_freq_x(n, 5) &&
	 is_freq_x(n, 6));
}

int main (int argc, char *argv[])
{
    u64 n;

    for (n = 10; n < 1000000; n++) {
	if (is_special_x(n))
	    printf("%Lu\n", n);
    }

    return 0;
}

