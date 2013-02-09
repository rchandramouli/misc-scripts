#include <stdio.h>

int main (int argc, char *argv[])
{
  int s = 0, i, m3, m5, m15;

  for (s = m3 = m5 = m15 = 0, i = 3; i < 1000; i++) {
    if ((i % 3) == 0)  m3++;
    if ((i % 5) == 0)  m5++;
    if ((i % 15) == 0) m15++;

    s += ((i % 3) == 0) ? i: ((i % 5) == 0) ? i: 0;
  }

  printf("%d\nm3 = %d, m5 = %d, m15 = %d", s, m3, m5, m15);
  return 0;
}
