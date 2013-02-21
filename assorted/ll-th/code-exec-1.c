#include <stdio.h>

static int fun1(int x, char sym, int y)
{
  printf("x = %d, sym = '%c', y = %d\n", x, sym, y);
  return 0;
}

static int fun2(char *name, int order)
{
  printf("name: \"%s\", order: %d\n", name, order);
  return 0;
}

struct data_fun1 {
  int x;
  char sym;
  int y;
};

struct data_fun2 {
  char *name;
  int order;
};

static int (*f[2])();

int main(int argc, char *argv[])
{
  struct data_fun1 df1 = { 100, 'x', 200 };
  struct data_fun2 df2 = { "mygirl", 4 };

  f[0] = (int (*)())fun1;
  f[1] = (int (*)())fun2;

  (*f[0])(df1);
  (*f[1])(df2);

  return 0; 
}
