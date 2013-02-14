#include <stdio.h>

#define MAXSIZE   300

int main (int argc, char *argv[])
{
	char ch[MAXSIZE];
	int l, i, j, s, c;

	while (scanf("%s", &ch[1]) > 0) {
		l = 1;
		while (ch[l++]);
		l-=2;

		if (l == 1 && (ch[1] == '1' || ch[1] == '0')) {
			printf("%c\n", ch[1]);
			continue;
		}

		/* N-1 */
		j = l;
		while (j) {
			if (ch[j]-'0') {
				ch[j]--;
				break;
			}
			ch[j] = '9';
			j--;
		}
		       
		/* 2*(N-1) */
		s = 0; j = l; c = 0;
		while (j) {
			i = (ch[j]-'0');
			s = (i << 1) + c;
			ch[j] = (s % 10) + '0';
			c = s/10;
			j--;
		}
		ch[0] = c+'0';
		printf("%s\n", (ch + (c^1)));
	}

	return 0;
}

