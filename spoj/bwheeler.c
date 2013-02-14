#include <stdio.h>
#include <strings.h>

int main (int argc, char *argv[])
{
	char cols[2][1004];
	int freq[26], pos[1004], r, l, i, j, k;

	while (1) {
		scanf("%d", &r);
		if (!r) break;
		scanf("%s", cols[0]);

		l = 0;
		for (i = 0; i < 26; i++) freq[i] = 0;
		while (cols[0][l]) {
			j = cols[0][l]-'a';
			pos[l] = freq[j];
			freq[j]++;
			l++;
		}
		j = 0;
		while (j < 26 && !freq[j++]);
		k = freq[j-1];
		freq[j-1] = 0;
		for (; j < 26; j++) {
			if (freq[j]) {
				i = freq[j];
				freq[j] = k;
				k += i;
			}
		}
		i = 0;
		j = r-1;
		while (i < l) {
			cols[1][l-i-1] = cols[0][j];
			j = freq[cols[0][j]-'a'] + pos[j];
			i++;
		}
		cols[1][i] = 0;
		printf("%s\n", cols[1]);
	}
	
	return 0;
}

