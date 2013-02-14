#include <stdio.h>

int main (int argc, char *argv[])
{
	char ch[1000], s[1000];
	int c, i, j, k, l;

	while (1) {
		scanf("%d", &c);
		if (!c) break;

		scanf("%s", ch);
		l = i = k = j = 0;
		while (ch[++l]);
		for (; i < c; i++, k = i) {
			s[j++] = ch[i];
			while (k < l) {
				k += ((c-i)<< 1)-1;
				(void)((k < l) && (s[j++] = ch[k]));
				k += (i<<1)+1;
				(void)((k < l) && (s[j++] = ch[k]));
			}
		}
		s[j] = 0;
		printf("%s\n", s);
	}
	
	return 0;
}

