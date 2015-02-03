#include <stdio.h>

int main (int argc, char *argv[])
{
  unsigned int n, t;

  scanf("%d", &n);

  for (t = n-2; t > 1; t--) {
    if (((t*t-1) % n) == 0)
      break;
  }
  printf("Max-Inverse < n-1 = %d\n", t);

  return 0;
}
