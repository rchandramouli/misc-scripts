#include <stdio.h>
#include <string.h>

typedef unsigned long long int u64;

static u64 sum;
static int divs[] = { 17, 13, 11, 7, 5, 3, 2 };
static int digit[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static int is_req_pandigit (u64 n)
{
	int i;

	for (i = 0; i < 7; i++) {
		if ((n % 1000) % divs[i])
			return 0;
		n /= 10;
	}
	return 1;
}
static u64 get_pandigit_sum (int n)
{
	int i;
	u64 t;

	if (n == 1) {
		t = 0;
		for (i = 0; i < 10; i++)
			t = (t*10) + digit[i];
		if (is_req_pandigit(t)) {
			printf(" >> %Lu\n", t);
			sum += t;
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
	get_pandigit_sum(10);

	printf("%Lu\n", sum);

	return 0;
}
