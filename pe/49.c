#include <stdio.h>
#include <string.h>

#define MAX_P    1000000
static char pf[MAX_P];
static int pt[MAX_P], nr;

static void sieve_p (void)
{
    int i, j, k = 0, a = 4;

    pt[k++] = 2;
    pt[k++] = 3;
    for (i = 5; i < MAX_P; i+=(a^=6)) {
	if (pf[i] == 1) continue;
	pt[k++] = i;
	for (j = i+(i<<1); j < MAX_P; j+=(i<<1))
	    pf[j] = 1;
    }
    nr = k;
}

static int is_perm (int a, int b, int c)
{
    int x[30], i;

    memset(x, 0, sizeof(x));

    while (a) { x[a%10]++; a/=10; }
    while (b) { x[10+b%10]++; b/=10; }
    while (c) { x[20+c%10]++; c/=10; }

    for (i = 0; i < 10; i++) {
	if (!((x[i] == x[10+i]) && (x[10+i] == x[20+i])))
	    return 0;
    }
    return 1;
}

static void get_primes (void)
{
    int i, p = 0, q = 0, x, y, z;

    for (i = 0; pt[i] < 1000; i++);
    p = i;
    for (; pt[i] < 10000; i++);
    q = i-1;

    for (x = p; x < q-1; x++) {
	for (y = x+1; y < q; y++) {
	    for (z = y+1; z < q+1; z++) {
		if (2*pt[y] == (pt[x]+pt[z]) &&
		    is_perm(pt[x], pt[y], pt[z]))
		    printf("%d, %d, %d\n", pt[x], pt[y], pt[z]);
	    }
	}
    }
}

int main (int argc, char *argv[])
{
    sieve_p();
    get_primes();

    return 0;
}

