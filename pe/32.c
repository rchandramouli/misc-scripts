#include <stdio.h>
#include <string.h>

typedef unsigned long long int u64;

static u64 sum;
static int digit[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, nr_pan_pdt;
static u64 pan_pdt[256];

#define MAX_PAN_DIGITS    9

static int is_req_pandigit (u64 n)
{
	int a, b, c, s = 0;

	c = n % 10000ULL;
	b = (n / 10000ULL) % 10000;
	a = (n / 100000000ULL) % 10;
	if (a*b == c) return c;

	b = (n / 10000ULL) % 1000;
	a = (n / 10000000ULL) % 100;
	if (a*b == c) return c;

	b = (n / 10000ULL) % 100;
	a = (n / 1000000ULL) % 1000;
	if (a*b == c) return c;

	return s;
}

static u64 get_pandigit_sum (int n)
{
	int i;
	u64 t, v;

	if (n == 1) {
		t = 0;
		for (i = 0; i < MAX_PAN_DIGITS; i++)
			t = (t*10) + digit[i];
		if ((v = is_req_pandigit(t))) {
			for (i = 0; i < nr_pan_pdt; i++)
				if (pan_pdt[i] == v)
					break;
			if (i == nr_pan_pdt) {
				pan_pdt[nr_pan_pdt++] = v;
				printf(" >> %Lu (%Lu)\n", t, v);
				sum += v;
			}
		}
		return 0;
	}

	for (i = 0; i < n; i++) {
		t = digit[n-1];
		digit[n-1] = digit[i];
		digit[i] = t;

		get_pandigit_sum(n-1);

		t = digit[n-1];
		digit[n-1] = digit[i];
		digit[i] = t;
	}
	return 0;
}

int main (int argc, char *argv[])
{
	get_pandigit_sum(MAX_PAN_DIGITS);

	printf("%Lu\n", sum);

	return 0;
}
