#include <stdio.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u32
#define MAX_PTABLE_SIZE   (100000UL)
#include "prime.h"

struct elem {
    u64 n;
    u32 left;
    u32 right;
    char valid;
};

#define MAX_PASCAL_ROWS  (51)
#define MAX_INDEX        (1326)

static struct elem el[MAX_INDEX];
static int free_index = 0;

static int is_square_free (u64 N)
{
    int i;

    for (i = 0; i < nr_primes; i++) {
        if ((N % ptable[i]) == 0) {
            if (((N / ptable[i]) % ptable[i]) == 0)
                return 0;
            N /= ptable[i];
        }
    }
    if (is_prime(N))
        return 1;

    /* FIXME: handle this better, but works now! */
    return 1;
}

static void insert_elem (u64 N)
{
    struct elem *t;

    t = &el[0];

    while (t->valid) {
        if (t->n == N)
            return;
        if (t->n > N) {
            if (!t->left) {
                t->left = free_index+1;
                t = &el[free_index++];
                t->n = N;
                t->valid = 1;
                return;
            }
            t = &el[t->left-1];
            
        } else if (t->n < N) {
            if (!t->right) {
                t->right = free_index+1;
                t = &el[free_index++];
                t->n = N;
                t->valid = 1;
                return;
            }
            t = &el[t->right-1];
        }
    }

    /* t is not valid, add N */
    t->n = N;
    t->valid = 1;
    free_index++;
}

u64 ptri[53];

int main (int argc, char *argv[])
{
    u64 S = 0;
    int i, j;

    ptri[0] = 1;
    ptri[1] = 1;

    timeit_timer_start();

    generate_primes();
    timeit_timer_peek_and_print();

    for (i = 3; i <= MAX_PASCAL_ROWS; i++) {
        for (j = i-1; j >= 0; j--) {
            ptri[j] = ptri[j] + ptri[j-1];
        }
        for (j = 1; j <= (i-1)/2; j++) {
            if (is_square_free(ptri[j])) {
                printf("Adding square-free %I64u at row %d\n",
                       ptri[j], i);
                insert_elem(ptri[j]);
                fflush(stdout);
            }
        }
    }
    timeit_timer_peek_and_print();

    for (i = 0; i < free_index; i++) {
        if (el[i].valid) {
            S += el[i].n;
        }
    }
    S += 1;
    timeit_timer_peek_and_print();
    printf("Sum of unique square-free numbers = %I64u\n", S);

    return 0;
}
