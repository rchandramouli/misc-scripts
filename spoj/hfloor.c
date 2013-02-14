#include <stdio.h>

#define MAX_SIZE  101

char room[MAX_SIZE][MAX_SIZE];
int m, n, p;

void count_people (int i, int j)
{
	if (i < 0 || i > m || j < 0 || j > n || room[i][j] == '#' || !room[i][j])
		return;

	p += (room[i][j] == '*');
	room[i][j] = 0;

	count_people(i+1, j);
	count_people(i-1, j);
	count_people(i, j+1);
	count_people(i, j-1);
}

int main (int argc, char *argv[])
{
	int t, r, total, i, j;

	scanf("%d", &t);
	while (t--) {
		scanf("%d %d", &m, &n);

		for (i = 0; i < m; i++)
			scanf(" %s", room[i]);

		p = r = total = 0;
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				if (room[i][j] && room[i][j] != '#') {
					++r;
					count_people(i, j);
					total += p;
					p = 0;
				}
			}
		}
		printf("%0.2lf\n", (double)total/r);
	}

	return 0;
}

