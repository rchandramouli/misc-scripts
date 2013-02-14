#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[])
{
  int n, f, r;
  char *c[] = { "N\n", "Y\n" };

  while (scanf("%d", &n) && n > 0) {
    n--;
    if (n%3) { f = 0; }
    else {
      r = sqrt(n/3);
      f = ((3*r*(r+1)) == n);
    }
    printf("%s", c[f]);
  }

  return 0;
}

