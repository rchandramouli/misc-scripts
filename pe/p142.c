#include <stdio.h>
#include <math.h>

#define FROM_BASE        5000
#define MAX_TRIES        10000

#define is_a_square(x)   ((x) == (((int)sqrt(x))*((int)sqrt(x))))

int main(int argc, char *argv[])
{
  int x, y, z;

  for (z = FROM_BASE; z < MAX_TRIES; z+=2) {
    for (y = z+2; y < MAX_TRIES; y+=2) {
      for (x = y+1; x < MAX_TRIES; x++) {
	if (is_a_square(x+y) && is_a_square(x-y) &&
	    is_a_square(y+z) && is_a_square(y-z) &&
	    is_a_square(x+z) && is_a_square(x-z)) {
	  printf("(%d %d %d) %d\n", x, y, z, x+y+z);
	  goto done;
	}
      }
    }
  }

 done:
  return 0;
}
