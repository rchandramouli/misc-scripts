#include <stdio.h>

#define MAXCHARS     5002

typedef unsigned long long int ull;

int main (int argc, char *argv[])
{
	char ch[MAXCHARS], pch, x;
	int i;
	ull p, v, pv, pvv;

	while (1) {
		scanf("%s", ch);
		if (ch[0] == '0') break;

		pv = pvv = i = v = 1;
		pch = ch[0]-'0';
		p = 1;

		while (ch[i] != 0) {
			x = pch*10 + (ch[i]-'0');
			if (ch[i] == '0') {
				p *= pvv;
				pv = pvv = v = 1;
				++i;
				if (ch[i] == 0)	break;
			} else if (x > 26) {
				p *= v;
				pv = pvv = v = 1;
			} else {
				v=pv+pvv;
				pvv=pv; pv=v;
			}
			pch = ch[i]-'0';
			i++;
		}
		if (pch != 0)
			p *= v;
		printf("%llu\n", p);
	}

	return 0;
}

