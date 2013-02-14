#include <stdio.h>
#include <string.h>

#define MAX_CHARS    1000002

int main (int argc, char *argv[])
{
	char ch[MAX_CHARS], x;
	int l, i, j, t, c;

	scanf("%d", &t);
	while (t--) {
		scanf("%s", &ch[1]);
		l = strlen(ch+1);

		j = 1 + ((l-1) >> 1);
		c = ch[0] = 0;
		for (i = j+1; i <= l && !c; i++) {
			c = (ch[i] >= ch[j-(i-j)+((l&1)^1)]);
		}
		if (c || (l == 1)) {
			c = 1;
			for (i = j; c && i > 0; i--) {
				x = ch[i]-'0'+c;
				ch[i]='0'+(x % 10);
				c=x/10;
			}
			ch[0] = (c) ? '1': 0;
		}
		for (i = j+1; i <= l; i++)
			ch[i] = ch[j-(i-j)+((l&1)^1)];
		ch[l+(c^1)] = ch[0];
		printf("%s\n", ch+(c^1));
	}

	return 0;
}

