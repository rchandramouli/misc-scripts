#include <stdio.h>
#include <string.h>

typedef unsigned int u32;
typedef unsigned long long int u64;

static u32 a[13], bnum[256], nr;

static void pc (void) {
  u32 i=0, p=1;
  for (; i < 12; i++) { a[i] = p; p *= (i+1); }
}

static void bnum_multiply (int n)
{
  u64 v = 0;
  u32 c = 0, i = 0;
  do {
    v = bnum[i];
    v = v*n + c;
    bnum[i] = (v % 1000000000ULL);
    c = v / 1000000000ULL;
    i++;
  } while (i < nr || v);
  nr = i-1;
}

static void big_factorial (int n)
{
  int i, j;

  nr = 1;
  memset(bnum, 0, sizeof(bnum));
  bnum[nr-1] = a[11];
  for (i = 12; i <= n; i++) {
    bnum_multiply(i);
  }
  printf("%u", bnum[nr-1]);
  for (j = nr-1; j > 0; j--)
    printf("%09u", bnum[j-1]);
  printf("\n");
}

int main (int argc, char *argv[]) {
  int t, n;

  pc();
  scanf("%d", &t);
  while (t--) {
    scanf("%d", &n);
    if (n < 12) printf("%u\n", a[n]);
    else {
      big_factorial(n);
    }
  }

  return 0;
}

