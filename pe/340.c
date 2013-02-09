#include <stdio.h>

typedef unsigned long long int u64;

static u64 a, b, c;

/*
 *    F(n) = n - c, for all n > b
 *    F(n) = F(a + F(a + F(a + F(a + n)))), for all n <= b
 *
 *    If a > c, then, it is evident that once n > b, then, while
 *    reiterating back in the recursion, we just need to add (a - c)
 *    to n. Below function assumes this.
 */
static inline u64 crazy_fn_norecurse (u64 n)
{
    u64 iter, k, n0;

    if (n > b) return (n - c);

    k = (b-n)/a;
    return (4*(a-c) + n + (4*a-3*c)*k);

    iter = 1;
    n0 = n;
    do {
	if (n0 > b) {
	    n0 += iter * (a - c);
	    iter = 0;
	} else {
	    k = (b - n0) / a;
	    iter += 3*(1+k);
	    n0 += (1+k)*a;
	}
    } while (iter);

    return (n0 - a);
}

int main (int argc, char *argv[])
{
    u64 i, v, sum;
    u64 c1, c2;

    scanf("%Lu %Lu %Lu", &a, &b, &c);

    c1 = (a*(a-1)/2);

    /* sum0 */
    i = 1 + (b % a);
    v = crazy_fn_norecurse(0);
    sum = (v % 10000000000ULL) * (i % 10000000000ULL);
    sum = (sum + (i * (i-1)/2)) % 1000000000000000ULL;
    printf("I: %Lu\n", i);

    for (; (i+a) <= b; ) {
	v = crazy_fn_norecurse(i);
	c2 = (v % 1000000000ULL) * (a % 100000000ULL);
	c2 = (c2 + c1) % 1000000000000000ULL;
	sum = (sum + c2) % 1000000000000000ULL;
	i += a;
    }

    if (i < b && ((i + a) > b)) {
	v = crazy_fn_norecurse(i);
	c1 = (v % 10000000000ULL) * ((b-i+1) % 10000000000ULL);
	c2 = ((b-i+1)*(b-i)/2) % 1000000000000000ULL;
	sum = (sum + c1 + c2) % 1000000000000000ULL;
    }
    printf("%Lu\n", sum % 1000000000ULL);

    return 0;
}
