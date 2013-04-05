#include <stdio.h>

static int ptable[] = { 2, 3, 5, 7, 11, 13, 17, 19, };

static void get_prime_factors(int N, int *f, int *cf)
{
  int i, c, k;

  f[0] = f[1] = f[2] = 0;
  cf[0] = cf[1] = cf[2] = 0;

  for (i = k = 0; i < sizeof(ptable)/sizeof(int); i++) {
    if ((N % ptable[i]) != 0) 
      continue;

    c = 0;
    f[k] = ptable[i];
    while (!(N % ptable[i])) {
      c++;
      N /= ptable[i];
    }
    cf[k] = c;
    k++;
  }

  if (!k || N > 1) {
    f[k] = N;
    cf[k] = 1;
  }
}

int main(int argc, char *argv[])
{
  int pf[201][3], cpf[201][3];
  int i;

  for (i = 2; i < 200; i++) {
    get_prime_factors(i, pf[i], cpf[i]);

    printf("%d:  ", i);
    if (pf[i][0]) printf("%3d[%3d] ", pf[i][0], cpf[i][0]);
    if (pf[i][1]) printf("%3d[%3d] ", pf[i][1], cpf[i][1]);
    if (pf[i][2]) printf("%3d[%3d] ", pf[i][2], cpf[i][2]);
    printf("\n");
  }

  return 0;
}
