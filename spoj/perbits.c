#include <stdio.h>

char bmap[8192];

#define set_bit(n)   bmap[n/8] |= (1<<((n)%8))
#define get_bit(n)   (bmap[n/8] & (1<<((n)%8)))

static void print_bits(unsigned int Z, unsigned int O)
{
  char v[17];
  int i = 15;

  do {
    v[i] = (Z&1) ? '0': (O&1) ? '1': '?';
    if (!i) break;
    Z >>= 1;
    O >>= 1;
    i--;
  } while (1);
  v[16] = 0;
  printf("%s\n", v);
}

int main(int argc, char *argv[])
{
  int a, b, c, s, x;
  unsigned int A, F, v;

  while (scanf("%d", &a) && a) {
    scanf("%d %d %d", &b, &c, &s);

    memset(bmap, 0, sizeof(bmap));

    A = F = 0xffffffff;
    v = s;

    while (!get_bit(v)) {
      set_bit(v);
      F &= ~v;
      A &= v;
      v = (a*v+b)%c;
    }
    print_bits(F, A);
  }

  return 0;
}

