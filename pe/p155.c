/* TBD */
#include <stdio.h>
#include <malloc.h>

#include "mytypes.h"
#include "timeit.h"

typedef double ld64;

struct lnode;

struct lnode {
    struct lnode *left, *right;
};

struct sorted_tree {
    ld64 key;
    struct lnode link;
};

struct lnode *root = NULL;

#define offset_of(T, m)  ((unsigned long)(&((T *)0)->m))

static inline struct sorted_tree *get_sorted_tree_obj (struct lnode *node)
{
    return (struct sorted_tree *)(((unsigned long)node) -
                                  offset_of(struct sorted_tree, link));
}

static inline void init_sorted_tree (struct sorted_tree *s, ld64 key)
{
    s->key = key;
    s->link.left = s->link.right = NULL;
}

static inline void add_node_to_node (struct sorted_tree *root,
                                     struct sorted_tree *node)
{
    if (root->key > node->key) {
        root->link.left = &node->link;
    } else if (root->key < node->key) {
        root->link.right = &node->link;
    }
}

static inline int add_to_sorted_tree (struct lnode **root, ld64 key)
{
    struct lnode *temp = *root;
    struct sorted_tree *node, *iter;

    if (*root == NULL) {
        node = malloc(sizeof(struct sorted_tree));
        *root = &node->link;

        init_sorted_tree(node, key);
        //printf("%s: adding key = %f\n", __func__, key);
        return 1;
    }
    while (1) {
        iter = get_sorted_tree_obj(temp);

        if (iter->key > key) {
            temp = temp->left;
        } else if (iter->key < key) {
            temp = temp->right;
        } else {
            break;
        }
        if (temp == NULL) {
            node = malloc(sizeof(struct sorted_tree));
            init_sorted_tree(node, key);
            //printf("%s: adding key = %f\n", __func__, key);
            add_node_to_node(iter, node);
            return 1;
        }
    }
    return 0;
}

static inline void dump_sorted_tree_node (struct sorted_tree *s)
{
    printf("KEY = %f\n", s->key);
}
    
static void dump_sorted_tree (struct lnode *root)
{
    if (!root)
        return;

    if (root->left)
        dump_sorted_tree(root->left);

    if (root->left == NULL) {
        dump_sorted_tree_node(get_sorted_tree_obj(root));
    }

    if (root->right)
        dump_sorted_tree(root->right);
}

#define CAP_SERIES   1
#define CAP_PARALLEL 2

#define CAP_MF    (1.0)

static u64 cap_count;

static void count_capacitors (int type, int n, ld64 C)
{
    if (n == 0)
        return;

    if (type == CAP_SERIES) {
        C = C / (CAP_MF + C);
    } else if (type == CAP_PARALLEL) {
        C = C + CAP_MF;
    }
    cap_count += add_to_sorted_tree(&root, C);

    count_capacitors(CAP_SERIES, n-1, C);
    count_capacitors(CAP_PARALLEL, n-1, C);
}


int main (int argc, char *argv[])
{
    int n;

    scanf("%d", &n);

    count_capacitors(CAP_SERIES, n, 1.0);

    printf("D(3) = %I64u\n", cap_count);

    return 0;
}
