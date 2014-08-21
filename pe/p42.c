#include <stdio.h>
#include <math.h>
#include <string.h>

static char words[10000][64];
static int nr;

static int is_triangle_num (int n)
{
    int sq;

    sq = sqrt(2*n);
    return (sq*(sq+1) == 2*n);
}

int main (int argc, char *argv[])
{
    char ch, w[64];
    int l, i, v, c = 0;

    while (scanf("%[^ ,\n]s", w) == 1) {
	l = strlen(w);
	memcpy(words[nr], w+1, l-2);
	nr++;
	if ((ch = getchar()) != ',')
	    break;
    }

    for (i = 0; i < nr; i++) {
	l = strlen(words[i]);
	v = 0;
	while (l) {
	    v += words[i][--l] - 'A' + 1;
	}

	if (is_triangle_num(v)) {
	    printf("%30s: %4d\n", words[i], v);
	    c++;
	}
    }

    printf("%d\n", c);

    return 0;
}
