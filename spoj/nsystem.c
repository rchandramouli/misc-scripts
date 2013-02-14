#include <stdio.h>

int main (int argc, char *argv[])
{
	int t, i, j;
	char n1[64], n2[64], m1[4], m2[4];

	scanf("%d", &t);
	while (t--) {
		n1[0] = n2[0] = 0;
		scanf(" %s %s", &n1[1], &n2[1]);

		*(int*)m1 = *(int*)m2 = 0;

		i = 0; j = -1;
		while (n1[++i]);
		while (--i) {
			switch(n1[i]) {
			case 'm': j++;
			case 'c': j++;
			case 'x': j++;
			case 'i': j++;
				if (n1[i-1] >= '0' && n1[i-1] <= '9') {
					m1[j] = n1[i-1]-'0';
					--i;
				} else {
					m1[j] = 1;
				}
				j = -1;
				break;
			}
		}
		i = 0; j = -1;
		while (n2[++i]);
		while (--i) {
			switch(n2[i]) {
			case 'm': j++;
			case 'c': j++;
			case 'x': j++;
			case 'i': j++;
				if (n2[i-1] >= '0' && n2[i-1] <= '9') {
					m2[j] = n2[i-1]-'0';
					--i;
				} else {
					m2[j] = 1;
				}
				j = -1;
				break;
			}
		}
		m1[0] += m2[0];
		m1[1] += m2[1] + m1[0]/10; m1[0] %= 10;
		m1[2] += m2[2] + m1[1]/10; m1[1] %= 10;
		m1[3] += m2[3] + m1[2]/10; m1[2] %= 10;
		m1[3] %= 10;

		if (m1[3]) { if (m1[3] > 1) printf("%d", m1[3]); printf("m"); }
		if (m1[2]) { if (m1[2] > 1) printf("%d", m1[2]); printf("c"); }
		if (m1[1]) { if (m1[1] > 1) printf("%d", m1[1]); printf("x"); }
		if (m1[0]) { if (m1[0] > 1) printf("%d", m1[0]); printf("i"); }
		printf("\n");
	}

	return 0;
}

