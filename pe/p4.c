#include <stdio.h>

static int is_palind (int n)
{
  int x = 0, t = n;

  while (t) {
    x = (x*10)+(t%10);
    t /= 10;
  }
  return (x == n);
}

int main (int argc, char *argv[])
{
  int x, y, pt;
  int mx, my, mpt = 0;

  for (x = 100; x < 1000; x++) {
    for (y = 100; y < 1000; y++) {
      pt = x*y;
      if (is_palind(pt)) {
	printf("%3d * %3d = %d\n", x, y, pt);

	if (mpt < pt) {
	  mx = x; my = y; mpt = pt;
	}
      }
    }
  }

  printf("\n%3d * %3d = %d\n", mx, my, mpt);

  return 0;
}
