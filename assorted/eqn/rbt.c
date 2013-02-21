#include <stdio.h>

static unsigned int get_reduced_byte(unsigned long int n)
{
  unsigned int w, x, y, z;
  unsigned int S = n;
  int c = 0;

  while (S > 0x100) {
    c++;
    if (c > 40) {
      printf("0x%8x does not reduce in 20 iterations, S = 0x%8x\n", n, S);
      break;
    }

    w = S & 0xff;
    x = (S >> 8) & 0xff;
    y = (S >> 16) & 0xff;
    z = (S >> 24) & 0xff;
    S = w*y + x*z;
  }

  return S;
}

int main(int argc, char *argv[])
{
  unsigned long int x;

  for (x = 0; x < 100000; x++)
    get_reduced_byte(x);

  return 0;
}
