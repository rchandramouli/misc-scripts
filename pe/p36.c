#include <stdio.h>
#include <string.h>

static int is_palindrome(char *s, char *e)
{
  while (s < e) {
    if (*s != *e)
      return 0;
    s++;
    e--;
  }

  return 1;
}

static int is_base10_palindrome(int t)
{
  char num[20], *p, *q;
  int i;

  i = 0;
  p = &num[0];
  while (t) {
    num[i] = (t%10)+'0';
    i++;
    t /= 10;
  }
  q = &num[i-1];

  return is_palindrome(p, q);
}

static int is_base2_palindrome(int t)
{
  char num[32], *p, *q;
  int i;

  i = 0;
  p = &num[0];
  while (t) {
    num[i] = (t & 1) + '0';
    i++;
    t >>= 1;
  }
  q = &num[i-1];

  return is_palindrome(p, q);
}

int main(int argc, char *argv[])
{
  int sum = 0, M, n;

  scanf("%d", &M);
  if (M <= 0) return -1;

  for (n = 1; n < M; n++) {
    if (is_base10_palindrome(n) && is_base2_palindrome(n)) {
      printf("%d\n", n);
      sum += n;
    }
  }

  printf("sum = %d\n", sum);

  return 0;
}
