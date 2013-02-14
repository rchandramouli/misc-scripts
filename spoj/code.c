#include <stdio.h>

#define MAX_STATES   1000000

char edge[MAX_STATES/10][10], str[MAX_STATES+10], *p;
int n, states, stk[MAX_STATES][2], ptr;

int main (int argc, char *argv[])
{
	int i, j, k;

	while (1) {
		scanf("%d", &n);
		if (!n) break;

		for (i = states = 1; i < n; i++) states *= 10;
		for (i = 0; i < states; i++) {
			edge[i][0] = edge[i][1] = edge[i][2] = 0;
			edge[i][3] = edge[i][4] = edge[i][5] = 0;
			edge[i][6] = edge[i][7] = edge[i][8] = 0;
			edge[i][9] = 0;
		}
		for (i = 1; i < n; i++)
			str[i-1] = '0';
		p = str + states*10 + n-1;
		str[states*10+n-1] = ptr = 0;

		/* Do DFS. */
		stk[ptr][0] = stk[ptr][1] = k = 0;
	_loop:
		j = stk[ptr][0];
		for (i = stk[ptr][1]; i < 10; i++) {
			if (!edge[j][i]) {
				edge[j][i] = 1;
				stk[ptr][1] = i+1;
				stk[++ptr][0] = (j*10+i) % states;
				stk[ptr][1] = 0;
				goto _loop;
			}
		}
		if (!ptr) goto _end;
		ptr--;
		*--p = '0'+stk[ptr][1]-1;
		goto _loop;
	_end:

		printf("%s\n", str);
	}
	return 0;
}

