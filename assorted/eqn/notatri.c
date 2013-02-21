#include <stdio.h>
#include <stdlib.h>

#define MAX_STICKS   2000

#define _l(n)      (((n)<<1)+1)
#define _r(n)      (((n)<<1)+2)
#define _p(n)      ((n)>>1)

static void heapify(int *arr, int k, int n)
{
  int l, r, m = k, t;

  while (1) {
    k = m;
    l = _l(m);
    r = _r(m);

    if (l < n && arr[l] > arr[m])
      m = l;
    if (r < n && arr[r] > arr[m])
      m = r;

    if (m == k) break;

    t = arr[m];
    arr[m] = arr[k];
    arr[k] = t;
  }      
}

static void heapsort(int *arr, int n)
{
  int i, t;

  for (i = (n-1)/2; i >= 0; i--)
    heapify(arr, i, n);

  for (i = n-1; i > 0; i--) {
    t = arr[0];
    arr[0] = arr[i];
    arr[i] = t;

    heapify(arr, 0, i);
  }
}

/* find k in arr, such that arr[i] >= k > arr[i-1] */
static int bsearch_min(int *arr, int n, int k)
{
  int f = 0, l = n-1, m = (f+l)/2;

  do {
    if (k > arr[m]) {
      f = m+1;
    } else {
      l = m-1;
    }
    m = (f+l)/2;
  } while (f < l);

  if (m <= 0) {
    m = 0;
  }
  if (k > arr[m]) {
    return (m+1 < n) ? m+1: -1;
  } else if (k == arr[m] && m > 0 && k < arr[m-1]) {
    return m-1;
  }
  return m;
}

static unsigned long long int nr_triangles(int *arr, int n)
{
  int i, j, mn;
  unsigned long long int c = 0;

  for (i = n-1; i > 1; i--) {
    for (j = i-1; j > 0; j--) {
      mn = bsearch_min(arr, j, arr[i]-arr[j]);

      if (mn < 0) break;
      c += j-mn;
    }
  }

  return ((unsigned long long int)n*(n-1)*(n-2)/6)-c;
}

int i_cmp(const void *a, const void *b)
{
  return (*(int *)a-*(int *)b);
}

int main(int argc, char *argv[])
{
  int trilen[MAX_STICKS], n, i;
  unsigned long long int v;

  while (1) {
    scanf("%d", &n);
    if (!n) break;

    for (i = 0; i < n; i++)
      scanf("%d", &trilen[i]);

    heapsort(trilen, n);
    
    printf("%Lu\n", nr_triangles(trilen, n));
  }

  return 0;
}
