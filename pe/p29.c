/* TBD */
#include <stdio.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"
#include "prime.h"

struct ptn_s {
    int n;
    int prime[3];
    int counts[3];
};

#define MAX_N    100

struct node {
    struct node *left, *right;
}

struct ehash_s {
    unsigned int Ekey;
    struct node enode;
}
        
struct phash_s {
    int Pkey;
    int n, ne;

    struct node *eroot;
    struct node pnode;
};

#define offset_of(T, m)      ((unsigned long)(&(((T *)0)->m)))
#define parent_of(T, a, m)   ((T *)((unsigned long)a - offset_of(T, m)))

#define UP_NODE_P(a)         parent_of(struct phash_s, a, pnode)
#define UP_NODE_E(a)         parent_of(struct phash_s, 

static struct node *proot = NULL;

static inline void init_ehash_enode (struct ehash_s *eh)
{
    eh->enode.left = eh->enode.right = NULL;
}

static inline void init_phash_pnode (struct phash_s *ph)
{
    ph->pnode.left = ph->pnode.right = NULL;
}

static struct phash_s *get_phash (int Pkey)
{
    struct phash_s *n;
    struct node *p;

    p = proot;
    while (t != NULL) {
        n = UP_NODE_P(p);
        if (n->Pkey < Pkey) {
            p = n->pnode.right;
        } else if (n->Pkey > Pkey) {
            p = n->pnode.left;
        } else {
            break;
        }
    }
    if (t == NULL) {
        n = calloc(sizeof(struct phash_s), 1);
        init_phash_pnode(n);
        n->Pkey = Pkey;
        if (root == NULL)
            proot = &n->pnode;
    }
    return n;
}

static void add_phash_node (struct phash_s *ph)
{
    struct phash_s *n;
    struct node *t;
    int i, is_right;

    if (root == NULL) {
        init_phash_node(ph);
        root = &ph->tnode;
        return;
    }

    t = root;
    is_right = 0;

    while (true) {
        n = UP_NODE(t);
        if (n->Pkey < ph->Pkey) {
            t = n->tnode.right;
            is_right = 1;
        } else if (n->Pkey < ph->Pkey) {
            t = n->tnode.left;
        } else {
            n->n = ph->n;
            memcpy(n->count[0], ph->count[0], sizeof(n->count[0]));
            memcpy(n->count[1], ph->count[1], sizeof(n->count[1]));
            memcpy(n->count[2], ph->count[2], sizeof(n->count[2]));
            break;
        }
        if (t == NULL) {
            init_phash_node(ph);
            if (is_right) {
                n->tnode.right = &ph->tnode;
            } else {
                n->tnode.left = &ph->tnode;
            }
            break;
        }
    }
}

#define PHASH(p1, p2, p3)      (10000*p1 + 100*p2 + p3)

static int phash (int p1, int p2, int p3)
{

int main (int argc, char *argv[])
{
    int a, b, i, j;

    timeit_timer_start();
    generate_primes();
    timeit_timer_peek_and_print();

    for (i = 0; ptable[i] <= MAX_N; i++) {
        do {
            j = ptable[i];
            for (k = j; k <= MAX_N; k += j) {
                ptN[k].prime[ptN[k].n] = ptable[i];
                ptN[k].counts[ptN[k].n]++;
            }
            j *= ptable[i];
        } while (j <= MAX_N);
        for (j = ptable[i]; j <= MAX_N; j += ptable[i])
            ptN[j].n++;
    }
    timeit_timer_peek_and_print();

    /* Add to tree */
    for (a = 2; a <= MAX_N; a++) {
        
    }
    for (a = 2; a <= MAX_N; a++) {
        for (b = 2; b <= MAX_N; b++) {

        }
    }
}
