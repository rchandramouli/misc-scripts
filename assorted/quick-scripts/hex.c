#include <stdio.h>

#define MAX_SIZE       128

int main (int argc, char *argv[])
{
	unsigned char thue[MAX_SIZE], w;
	int i, b, j;

	thue[0] = 0x69;
	for (b = 1; b < 128; b *= 2) {
		for (j = 0; j < b; j++)
			thue[b+j] = thue[j] ^ 0xff;
	}

	for (b = 8, i = 0; i < MAX_SIZE; ) {
		if (b == 8) {
			w = thue[i];
		} else {
			w = (thue[i] << (8-b)) | (thue[i+1] >> b);
		}
		printf("%02X ", w);
		if (b == 1) printf("\n");
		b = 1 + ((6 + b) % 8);
		if (b == 8) i++;

	}
	printf("\n");

	return 0;
}
