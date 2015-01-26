#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int arr[30];

static inline void swap (int e1, int e2)
{
  int t = arr[e1];

  arr[e1] = arr[e2];
  arr[e2] = t;
}

static void check_array (void)
{
  int i, err = 0;

  for (i = 0; i < 30-1; i++) {
    if (arr[i] > arr[i+1])
      err = 1;
    break;
  }
  if (err) {
    printf("\n-------- ERRORED ---------\n");
  }
}

static void dump_array (int tabs, char *pfx)
{
  int i;

  for (i = 0; i < tabs; i++)
    printf("\t");
  if (pfx)
    printf("%s: ", pfx);
  for (i = 0; i < 30; i++)
    printf("%2d ", arr[i]);
  printf("\n");
  fflush(stdout);
}

static int partition_array (int from, int to)
{
  int pivot = (from + to)/2;
  int pv = arr[pivot];
  int i, j, count;

  //printf("\tPIVOT POINT = %d, value = %d\n", pivot, pv);

  swap(from, pivot);
  //dump_array(2, "pivot");

  i = from+1;
  j = to;

  count = 0;
  for (;;) {
    while (arr[i] <= pv && i < j)
      i++;
    while (arr[j] >= pv && j > i)
      j--;
    if (i == j)
      break;
    swap(i, j);

    /* Catch infinite loops! */
    count++;
    if (count == 300) {
      printf("%s: pivot = %d, from = %d, to = %d, pv = %d, arr[i=%d] = %d, arr[j=%d] = %d\n",
             __func__, pivot, from, to, pv, i, arr[i], j, arr[j]);
      dump_array(2, "error");
      exit(-1);
    }
    //dump_array(2, "loop ");
  }
  if (i == to && arr[i] < arr[from]) {
    swap(from, to);
    //dump_array(2, "fina1");
    return i;
  } else {
    swap(from, j-1);
    //dump_array(2, "fina2");
    return j-1;
  }
}

static void qsort_array (int from, int to)
{
  if (from < to) {
    int pivot = partition_array(from, to);

    //printf("%s: from = %d, pivot = %d, to = %d (fixed: %d)\n",
    //__func__, from, pivot, to, arr[pivot]);

    qsort_array(from, pivot-1);
    qsort_array(pivot+1, to);
  }
}


int main (int argc, char *argv[])
{
  int i;

  srand(time(NULL));

  for (i = 0; i < 30; i++)
    arr[i] = rand() % 100;

  dump_array(0, NULL);

  qsort_array(0, 29);

  dump_array(0, NULL);

  check_array();

  return 0;
}
