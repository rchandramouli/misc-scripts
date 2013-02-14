/* Cool question. The first player always wins. */
#include <stdio.h>

int main (int argc, char *argv[])
{
	int t, n, p;
	char *win[] = { "Airborne", "Pagfloyd" };

	scanf("%d", &t);
	while (t--) {
		scanf("%d %d", &n, &p);

		printf("%s wins.\n", win[p]);
	}

	return 0;
}

