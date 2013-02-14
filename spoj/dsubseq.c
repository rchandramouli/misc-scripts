/* Count all distinct subsequences. DP. */
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH  100005
#define MOD         1000000007
#define MAXCHARS    256

int main (int argc, char *argv[])
{
	unsigned long int tcases, diff[MAXCHARS], count, prev, i = 0, v;
	unsigned char str[MAX_LENGTH];

	scanf("%lu", &tcases);
	while (tcases--) {
		str[0] = 0;
		scanf("%s", str);
		bzero(diff, MAXCHARS * 4);

		i = 0;
		prev = count = 1;
		for (; str[i] != '\0'; i++) {
			count = (MOD + prev * 2 - diff[(int)str[i]]) % MOD;
			diff[(int)str[i]] = prev;
			prev = count;
		}

		printf("%lu\n", count);
	}

	return 0;
}

