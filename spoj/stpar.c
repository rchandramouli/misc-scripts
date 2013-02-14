/* Interesting dude! */
#include <stdio.h>
#include <strings.h>

#define MAX 1010

int main (int argc, char *argv[])
{
	char instk[MAX];
	int n, i, j, stk[MAX], q[MAX], head, stkmax = MAX;

	stk[0] = MAX;
	while (1) {
		scanf("%d", &n);
		if (!n) break;

		bzero(instk, sizeof(char)*MAX);
		i = 1;
		while (i <= n) {
			scanf("%d", &q[i]);
			i++;
		}

		head = 0;
		stkmax = stk[head];
		i = j = 1;
		for (; i <= n; ) {
			if (q[j] == i) {
				i++;
				j++;
			} else if (stkmax == i) {
				instk[stkmax] = 0;
				if (head)
					stkmax = stk[--head];
				i++;
			} else if (instk[i] || stkmax < q[j]) {
				break;
			} else {
				stkmax = q[j];
				instk[stkmax] = 1;
				stk[++head] = stkmax;
				j++;
			}
		}
		printf("%s\n", (i>n) ? "yes" : "no");
	}

	return 0;
}

