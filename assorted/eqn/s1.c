#include <stdio.h>

/* f(n) = 1 + f(f(n-1)) - f(f(n-2)), f(0) = 0, f(1) = 1 */
static int f1_values[1024];
static int eval_f1(int n)
{
  if (n < 2) return n;
  //  f1_values[n] = 1+f1_values[f1_values[n-1]]-f1_values[f1_values[n-2]];
  f1_values[n] = 1+f1_values[n-1]-f1_values[n-2];

  return f1_values[n];
}

static int f2_values[1024];
static int eval_f2(int n)
{
  if (n < 4) return 0;
  f2_values[n] = 1+f2_values[n-1]-f2_values[n-2]+f2_values[n-3]-f2_values[n-4];

  return f2_values[n];
}

static void do_eval_f1(void)
{
  int n;

  f1_values[0] = 0;
  f1_values[1] = 1;

  for (n = 2; n < 30; n++) {
    eval_f1(n);
  }
  for (n = 0; n < 30; n++) {
    printf("%2d ", f1_values[n]);
  }
  printf("\n");
}

static void do_eval_f2(void)
{
  int n;

  f2_values[0] = f2_values[1] = f2_values[2] = f2_values[3] = 0;

  for (n = 4; n < 40; n++) {
    eval_f2(n);
  }
  for (n = 0; n < 40; n++) {
    printf("%2d ", f2_values[n]);
  }
  printf("\n");
}

static int fvals[4096];
static void do_generic_feval(int k, int n)
{
  int i, j, r, s;

  for (i = 0; i < k; i++)
    fvals[i] = i;

  for (i = k; i < n; i++) {
    r = s = 1;
    for (j = 0; j < k; j++) {
      r += s*fvals[i-1-j];
      s = -s;
    }
    fvals[i] = r;
  }

  for (i = 0; i < n; i++) {
    printf("%3d ", fvals[i]);
    if (((i+1) % 15) == 0)
      printf("\n");
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
  int k, n;

  scanf("%d %d", &k, &n);

  //do_eval_f1();
  //do_eval_f2();

  do_generic_feval(k, n);

  return 0;
}
