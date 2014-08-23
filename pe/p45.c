#include <stdio.h>

typedef unsigned long long int u64;

static u64 get_next_tri (u64 *n, u64 N)
{
    u64 m, k;

    k = *n+1;
    m = (k*(k+1))/2;

    while (m < N) {
	k++;
	m = (k*(k+1))/2;
    }

    *n = k;
    return m;
}

static u64 get_next_pent (u64 *n, u64 N)
{
    u64 m, k;

    k = *n+1;
    m = (k*(3*k-1))/2;

    while (m < N) {
	k++;
	m = (k*(3*k-1))/2;
    }

    *n = k;
    return m;
}

static u64 get_next_hex (u64 *n, u64 N)
{
    u64 m, k;

    k = *n+1;
    m = (k*(2*k-1));

    while (m < N) {
	k++;
	m = (k*(2*k-1));
    }

    *n = k;
    return m;
}

int main (int argc, char *argv[])
{
    u64 n1, n2, n3;
    u64 m1, m2, m3;
    u64 c = 0;

    n1 = n2 = n3 = 1;
    m1 = m2 = m3 = 1;

    while (c < 2) {
	m3 = get_next_hex(&n3, m3);
	m1 = get_next_tri(&n1, m3);
	m2 = get_next_pent(&n2, m3);

	//printf("%Lu %Lu %Lu --- %Lu %Lu %Lu\n", n1, n2, n3, m1, m2, m3);
	if (m1 == m2 && m2 == m3) {
	    printf("%Lu %Lu %Lu => %Lu\n",
		   n1, n2, n3, m1);
	    c++;
	}
    }

    return 0;
}
