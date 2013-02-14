#include <stdio.h>

#define  MAXSIZE   300

int main (int argc, char *argv[])
{
	char ch[MAXSIZE];
	int zeros[MAXSIZE+1], cuts[MAXSIZE][MAXSIZE];
	int n, m, p, d, t, i, j, k;

	scanf("%d", &t);
	while (t--) {
		scanf("%d %s", &n, ch);

		zeros[0] = 0;
		for (i = 0; i < n; i++) {
			zeros[i+1] = zeros[i] + (('1' - ch[i]) ? -1: 1);
			cuts[i][i] = ch[i] - '0';
		}
		if (zeros[n] > 0) {
			printf("%d\n", n);
			continue;
		}
		for (i = 1; i < n; i++) {
			for (j = 0; j+i < n; j++) {
				m = 0;
				d = zeros[j+i+1]-zeros[j];
				if (d > 0) {
					m = i+1;
				} else {
					for (k = j; k < j+i; k++) {
						p = cuts[j][k] + cuts[k+1][j+i];
						if (p > m)
							m = p;
					}
				}
				cuts[j][j+i] = m;
			}
		}
		printf("%d\n", cuts[0][n-1]);
	}
	
	return 0;
}

