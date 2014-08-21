#include <stdio.h>
#include <string.h>

static int get_recur_length (int n)
{
    int digit[1000];
    int q, r, i;

    memset(digit, 0x00, sizeof(digit));

    i = 1; q = 1;
    while (1) {
	r = q % n;
	if (r == 0 || digit[r])
	    break;

	digit[r] = i;
	i++;
	q = r*10;
    }
    if (r) {
	return (i-digit[r]);
    }

    return 0;
}

int main (int argc, char *argv[])
{
    int i, l, mx = 0, mi;

    for (i = 1; i < 1000; i++) {
	l = get_recur_length(i);
	if (l > mx) {
	    mi = i;
	    mx = l;
	}
    }

    printf("%d %d\n", mi, mx);

    return 0;
}
