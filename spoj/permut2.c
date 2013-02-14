#include <stdio.h>

#define MAX 100001

int main (int argc, char *argv[])
{
	int n, arr[MAX], f, i;

	while (1) {
		scanf("%d", &n);
		if (!n) break;

		f = 1;
		for (i = 0; i < n; i++)
			scanf("%d", &arr[i]);

		for (i = 0; i < n && f; i++) {
			f = (arr[arr[i]-1] == i+1);
		}
		printf("%s\n", (f) ? "ambiguous": "not ambiguous");
	}

	return 0;
}

