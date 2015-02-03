/* 2^2470637 = 2^8 = 256 (mod 1475789056xb) */

#include <stdio.h>

#include "mytypes.h"

#define MAX_INDEX  (10000000)

struct elem {
    u64 pwr;
    u64 n;
    u32 left;
    u32 right;
    char valid;
};

static struct elem el[MAX_INDEX];
static int free_index = 0;

static int insert_elem (u64 pwr, u64 N, struct elem **ret)
{
    struct elem *t;

    if (free_index >= MAX_INDEX)
        return -1;

    t = &el[0];
    while (t->valid) {
        if (t->n == N) {
            *ret = t;
            return 1;
        }
        if (t->n > N) {
            if (!t->left) {
                t->left = free_index+1;
                t = &el[free_index++];
                t->n = N;
                t->pwr = pwr;
                t->valid = 1;
                return 0;
            }
            t = &el[t->left-1];
            
        } else if (t->n < N) {
            if (!t->right) {
                t->right = free_index+1;
                t = &el[free_index++];
                t->n = N;
                t->pwr = pwr;
                t->valid = 1;
                return 0;
            }
            t = &el[t->right-1];
        }
    }

    /* t is not valid, add N */
    t->n = N;
    t->pwr = pwr;
    t->valid = 1;
    free_index++;
    return 0;
}

#define MODN  (1475789056ULL)

int main (int argc, char *argv[])
{
    struct elem *tmp;
    u64 pdt = 1, n;
    int rc;

    n = 0;
    insert_elem(0, 1, &tmp);

    while (1) {
        pdt = (pdt * 2) % MODN;
        n++;
        rc = insert_elem(n, pdt, &tmp);
        if (rc == 1) {
            printf("2^%I64d = 2^%I64d = %I64d (mod %I64d)\n",
                   n, tmp->pwr, tmp->n, MODN);
            break;
        } else if (rc == -1) {
            printf("Not enough space in the tree, over %d "
                   "elements and counting!\n", n);
            break;
        }
    }
    return 0;
}
