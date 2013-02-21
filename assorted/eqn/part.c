#include <stdio.h>

#define NBUCKETS   32

static int buckets[NBUCKETS], N;

static int count_parts(int k)
{
  int C = 0, t = 0, i, y = 0;

  if (!k || buckets[k] == N) return 0;

  for (i = k; i > 0; i--, t=0) {
    if (!buckets[i])
      continue;

    while (buckets[i]) {
      t++;
      buckets[i]--;
      buckets[i-1] += 2;
      C += count_parts(i-1);
    }
    y += t;
    buckets[i] += t;
    buckets[i-1] -= 2*t;
  }

  return C+y;
}

static int g2parts(void)
{
  int i, k = 0, t;

  for (i = k = 0, t = N; t && i < NBUCKETS; i++, t=t>>1) {
    buckets[i] = (t & 1);
  }
  k = i-1;

  return (count_parts(k)+(N&1));
}

int main(int argc, char *argv[])
{
  scanf("%d", &N);
  if (N <= 0) return -1;

  printf("%d\n", g2parts());

  return 0;
}
