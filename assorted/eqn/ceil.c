#include <stdio.h>

int main(int argc, char *argv[])
{
  int f, t, s;
  int S, i, x, sh, v;

  scanf("%d %d %d", &f, &t, &s);
  if (s == 0 || (f > t && s > 0) || (f < t && s < 0)) return -1;

  for (i = f; i < t; i += s) {
    S = v = 0;
    sh = 1;
    x = i;

    do {
      v = (x >> sh) + ((x & ((1 << sh)-1)) ? 1: 0);
      S += v;
      sh++;
    } while (v != 1);

    printf("%d %d %d\n", i, S, i-S);
  }

  return 0;
}
