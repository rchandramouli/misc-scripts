#include <stdio.h>

int main (int argc, char *argv[])
{
	int s, f = 1;

	while (scanf("%d", &s) > 0) {
		if (s == 42) f = 0;
		(void)(f && printf("%d\n", s));
	}
     
	return 0;
}

