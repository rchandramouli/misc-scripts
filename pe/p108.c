#include <stdio.h>

/* INCOMPLETE: Fixme, reduce time */

int main(int argc, char *argv[])
{
  int n, t, x, y, c, f = 0;

  scanf("%d", &t);
  if (t <= 0) return -1;

  for (n = t/2; ; n++) {
    y = c = 0;
    for (x = n+1; ; x++) {
      if (((n*x) % (x-n))) continue;
      y = (n*x)/(x-n);
      if (x > y) break;
      c++;
      printf("\tn = %d, x = %d, y = %d\n", n, x, y);
    }
    if (c >= t) {
      f = 1;
      break;
    }
    printf("n = %d, c = %d\n", n, c);
  }

  if (f) printf("%d\n", n);

  return 0;
}
