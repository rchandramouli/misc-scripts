#include <stdio.h>
#include <math.h>

static char pflag[1000000];
static int ptable[1000000], nr;

static void generate_primes (void)
{
  int i = 2, j;

  while (i < 1000000) {
    if (pflag[i] == 1) {
      i++;
      continue;
    }
    ptable[nr++] = i;

    for (j = i+i; j < 1000000; j+=i) {
      pflag[j] = 1;
    }
    i++;
  }
}

int main (int argc, char *argv[])
{
  unsigned long long int n, i, np, mx = 0;

  generate_primes();

  scanf("%Lu", &n);
  if (n <= 1) return -1;

  if (n < nr) {
    printf("n-th prime = %d\n", ptable[n]);
  }

  np = sqrt(n) + 1;
  for (i = 0; i < np && i < nr; i++) {
    if ((n % ptable[i]) == 0) {
      if (mx < ptable[i]) mx = ptable[i];
      printf("%d\n", ptable[i]);
    }
  }

  mx = (mx == 0) ? n: mx;
  printf("\nmax factor = %Lu\n", mx);

  return 0;
}
