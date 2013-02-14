#include <stdio.h>
#include <strings.h>

int main (int argc, char *argv[])
{
	char ch[1004];
	int C[10], i, j, t, s, f;

	scanf("%d", &t);
	while (t--) {
		scanf("%s", &ch[0]);

		bzero(&C, sizeof(int)*10);
		i = f = s = 0;
		while (ch[i]) {
			C[ch[i]-'0']++;
			s = (s + ch[i++]-'0') % 3;
		}
		if ((!C[0] && !C[5])) {
			printf("impossible\n");
			continue;
		}
		if (!C[0] && C[5]) {
			f = 1;
			C[5]--;
		}
		if (s == 1) {
			/* 1 4 7 22 25 55 28 58 88 */
			if (C[1]) C[1]--;
			else if (C[4]) C[4]--;
			else if (C[7]) C[7]--;
			else if (C[2] > 1) C[2] -= 2;
			else if (C[2] && C[5]) { C[2]--; C[5]--; }
			else if (C[5] > 1) C[5] -= 2;
			else if (C[2] && C[8]) { C[2]--; C[8]--; }
			else if (C[5] && C[8]) { C[5]--; C[8]--; }
			else if (C[8] > 1) C[8] -= 2;
			else s = -1;
		} else if (s == 2) {
			/* 2 5 8 11 14 44 17 47 77 */
			if (C[2]) C[2]--;
			else if (C[5]) C[5]--;
			else if (C[8]) C[8]--;
			else if (C[1] > 1) C[1] -= 2;
			else if (C[1] && C[4]) { C[1]--; C[4]--; }
			else if (C[4] > 1) { C[4] -= 2; }
			else if (C[1] && C[7]) { C[1]--; C[7]--; }
			else if (C[4] && C[7]) { C[4]--; C[7]--; }
			else if (C[7] > 1) C[7] -= 2;
			else s = -2;
		}
		if (s < 0) {
			printf("impossible\n");
			continue;
		}
		for (i = 9, j = 0; i > 0; i--)
			for (s = 0; s < C[i]; s++)
				ch[j++] = '0' + i;
		if (j == 0) {
			printf("0\n");
		} else {
			for (s = 0; s < C[0]; s++)
				ch[j++] = '0';
			if (f) ch[j++] = '5';
			ch[j] = 0;
			printf("%s\n", ch);
		}
	}

	return 0;
}

