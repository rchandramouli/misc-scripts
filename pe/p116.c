#include <stdio.h>

#define MAX_BLOCKS    100

typedef unsigned long long u64;

static u64 r[MAX_BLOCKS] = { 1, 2, 3, 5 };
static u64 g[MAX_BLOCKS] = { 1, 1, 2, 3 };
static u64 b[MAX_BLOCKS] = { 1, 1, 1, 2 };

int main(int argc, char *argv[])
{
  int i, l;

  scanf("%d", &l);
  if (l <= 0) return -1;

  for (i = 4; i < l; i++) {
    r[i] = r[i-1]+r[i-2];
    g[i] = g[i-1]+g[i-3];
    b[i] = b[i-1]+b[i-4];
  }

  printf("%llu\n", r[i-1]+g[i-1]+b[i-1]-3);

  return 0;
}
