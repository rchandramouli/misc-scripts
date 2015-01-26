#include <stdio.h>

struct tupleblock {
  int ppwr[10];
};

struct tupleblock tp[] = {
  [0] = { .ppwr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, },
  [1] = { .ppwr = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2}, },
};

int main (int argc, char *argv[])
{
  return 0;
}
