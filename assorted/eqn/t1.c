#include <stdio.h>
#include "tloop.h"

int main (int argc, char *argv[])
{
  int l[MX], j, k;
  int c = 0;

  FMX_N {
    F(j,MX,k=0) k |= (1<<l[j]);
    if (k == (((1<<(MX-1))-1)<<1)) {
      printf("(%d", l[0]);
      F(j,MX,1) printf(",%d", l[j]);
      printf(")\n");
      c++;
    }
  }
  printf("%d\n", c);
  return 0;
}
