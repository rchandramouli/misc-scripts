#include <stdio.h>
#include <string.h>

typedef struct {
    int nr;
    unsigned int digits[1024];
} bnum_t;

typedef unsigned long long int u64;

/* c = a+b */
static void bnum_add (bnum_t *a, bnum_t *b, bnum_t *c)
{
    int i = 0;
    u64 sum = 0;

    while (i < a->nr && i < b->nr) {
	sum += a->digits[i] + b->digits[i];
	c->digits[i] = sum % 1000000000;
	sum /= 1000000000;
	i++;
    }
    if (i < a->nr) {
	sum += a->digits[i];
	c->digits[i] = sum % 1000000000;
	sum /= 1000000000;
	i++;
    }
    if (i < b->nr) {
	sum += b->digits[i];
	c->digits[i] = sum % 1000000000;
	sum /= 1000000000;
	i++;
    }
    while (sum) {
	c->digits[i] = sum % 1000000000;
	sum /= 1000000000;
	i++;
    }
    c->nr = i;
}

static void bnum_dump (bnum_t *a)
{
    int i;

    for (i = a->nr-1; i >= 0; i--)
	printf("%d", a->digits[i]);
}

static int bnum_digit_sum (bnum_t *a)
{
    int sum = 0, t, i;

    for (i = 0; i < a->nr; i++) {
	t = a->digits[i];
	while (t) {
	    sum += t % 10;
	    t /= 10;
	}
    }

    return sum;
}

int main (int argc, char *argv[])
{
    bnum_t a, b, *c[2];
    int i, x;

    c[0] = &a;
    c[1] = &b;

    memset(&a, 0x00, sizeof(a));
    memset(&b, 0x00, sizeof(b));

    a.nr = 1;
    a.digits[0] = 536870912;

    for (i = 1, x = 0; i < 1000-28; i++, x^=1) {
	bnum_add(c[x], c[x], c[x^1]);
    }
    bnum_dump(c[x]);

    printf("\n\n%d\n", bnum_digit_sum(c[x]));

    return 0;
}
