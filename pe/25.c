#include <stdio.h>
#include <string.h>

typedef struct {
    int nr;
    unsigned int digits[1024];
} bnum_t;

typedef unsigned long long int u64;

static void bnum_dump (bnum_t *a)
{
    int i;

    for (i = a->nr-1; i >= 0; i--)
	printf("%09d ", a->digits[i]);
    printf("\n");
}

/* c = a+b */
static void bnum_add (bnum_t *a, bnum_t *b, bnum_t *c)
{
    int i = 0;
    u64 sum = 0;

    while (i < a->nr && i < b->nr) {
	sum += a->digits[i] + b->digits[i];
	c->digits[i] = sum % 1000000000ULL;
	sum /= 1000000000ULL;
	i++;
    }
    if (i < a->nr) {
	sum += a->digits[i];
	c->digits[i] = sum % 1000000000ULL;
	sum /= 1000000000ULL;
	i++;
    }
    if (i < b->nr) {
	sum += b->digits[i];
	c->digits[i] = sum % 1000000000ULL;
	sum /= 1000000000ULL;
	i++;
    }
    while (sum) {
	c->digits[i] = sum % 1000000000ULL;
	sum /= 1000000000ULL;
	i++;
    }
    c->nr = i;
}

static int bnum_count_digits (bnum_t *a)
{
    int count, t;

    count = (a->nr-1) * 9;
    t = a->digits[a->nr-1];
    while (t) {
	count++;
	t /= 10;
    }
    return count;
}

int main (int argc, char *argv[])
{
    bnum_t f0, f1, f2, *fib[3];
    int n1, n2, n3, x;

    f0.nr = f1.nr = 1;
    f0.digits[0] = f1.digits[0] = 1;
    fib[0] = &f0;
    fib[1] = &f1;
    fib[2] = &f2;

    n1 = 0; n2 = 1; n3 = 2; x = 3;

    while (1) {
	bnum_add(fib[n1], fib[n2], fib[n3]);

	if (bnum_count_digits(fib[n3]) == 1000)
	    break;

	n1 = (1+n1)%3;
	n2 = (1+n2)%3;
	n3 = (1+n3)%3;
	x++;
    }

    bnum_dump(fib[n3]);
    printf("\n%d\n", x);

    return 0;
}
