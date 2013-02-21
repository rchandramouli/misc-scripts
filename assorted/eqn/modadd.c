#include <stdio.h>
#include <malloc.h>

static int *tab, p, r, c;

static inline void dump_row(void)
{
	int i;

	for (i = 0; i < c; i++)
		printf("%2d ", tab[i]);
	printf("\n");
}

static inline void recalc_row(void)
{
	int i;

	for (i = 1; i < c; i++)
		tab[i] = (tab[i] + tab[i-1]) % p;
}

int main(int argc, char *argv[])
{
	int i;

	scanf("%d %d %d", &p, &r, &c);
	if (r <= 0 || c <= 0 || p <= 2)
		return -1;

	tab = calloc(sizeof(int), c);
	if (!tab) {
		perror("calloc");
		return -1;
	}

	i = r;
	tab[0] = 1;

	while (i) {
		dump_row();
		recalc_row();
		i--;
	}

	return 0;
}
