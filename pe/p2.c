#include <stdio.h>

int main (int argc, char *argv[])
{
  unsigned int s, f0, f1, f2;

  s = 0;
  f0 = 1;
  f1 = 1;
  f2 = f0 + f1;

  while (f2 < 4000000) {
    s += f2;

    f0 = f1+f2;
    f1 = f0+f2;
    f2 = f0+f1;
  }

  printf("%u (%u %u %u)\n", s, f0, f1, f2);

  return 0;
}
