#include <stdio.h>

#include "bnum.h"

int main (int argc, char *argv[])
{
    bnum_t f;
    int n;

    f.nr = 1; f.digits[0] = 1;

    for (n = 2; n <= 100; n++) {
	bnum_multiply_num(&f, n);
	bnum_dump(&f);
    }

    printf("\n%d\n", bnum_digit_sum(&f));

    return 0;
}
