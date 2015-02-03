#include <stdio.h>
#include <malloc.h>

typedef unsigned long long int u64;

struct lnode;

struct lnode {
    struct lnode *left, *right;
};

#define MAX_ELEMS   10

struct sorted_tree {
    u64 key;
    int n_elems;
    int elem[MAX_ELEMS];

    struct lnode link;
};

#define offset_of(T, m)  ((unsigned long)(&((T *)0)->m))

static inline struct sorted_tree *get_sorted_tree_obj (struct lnode *node)
{
    return (struct sorted_tree *)(((unsigned long)node) -
                                  offset_of(struct sorted_tree, link));
}

static inline void init_sorted_tree (struct sorted_tree *s, u64 key)
{
    s->key = key;
    s->n_elems = 0;
    s->link.left = s->link.right = NULL;
}

static inline int add_elem_to_node (struct sorted_tree *s, int elem)
{
    if (s->n_elems >= MAX_ELEMS) {
        return -1;
    }
    s->elem[s->n_elems++] = elem;
    return s->n_elems;
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

static inline int add_to_sorted_tree (struct lnode **root, u64 key, int elem)
{
    struct lnode *temp = *root;
    struct sorted_tree *node, *iter;
    int nelem = 0;

    if (*root == NULL) {
        node = malloc(sizeof(struct sorted_tree));
        *root = &node->link;

        init_sorted_tree(node, key);
        return add_elem_to_node(node, elem);
    }
    while (1) {
        iter = get_sorted_tree_obj(temp);

        if (iter->key > key) {
            temp = temp->left;
        } else if (iter->key < key) {
            temp = temp->right;
        } else {
            return add_elem_to_node(iter, elem);
        }
        if (temp == NULL) {
            node = malloc(sizeof(struct sorted_tree));
            init_sorted_tree(node, key);
            nelem = add_elem_to_node(node, elem);
            add_node_to_node(iter, node);
            break;
        }
    }
    return nelem;
}
static inline void dump_sorted_tree_node (struct sorted_tree *s)
{
    int i;
    u64 c;

    printf("KEY = %I64u (elems = %d)", s->key, s->n_elems);
    for (i = 0; i < s->n_elems; i++) {
        c = (u64)s->elem[i] * s->elem[i] * s->elem[i];
        printf("\n\t%5d  %I64u", s->elem[i], c);
    }
    printf("\n");
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

static void dump_sorted_tree_by_key (struct lnode *root, u64 key)
{
    struct sorted_tree *s;
    struct lnode *temp;

    if (!root)
        return;

    temp = root;
    while (temp) {
        s = get_sorted_tree_obj(temp);
        if (s->key == key) {
            dump_sorted_tree_node(s);
            break;

        } else if (s->key > key) {
            temp = temp->left;

        } else if (s->key < key) {
            temp = temp->right;
        }
    }
}

static u64 get_desc_sorted_number (u64 n)
{
    int d[10], i;

    for (i = 0; i < 10; i++)
        d[i] = 0;

    while (n) {
        d[n % 10]++;
        n /= 10;
    }
    for (i = 9; i >= 0; i--) {
        while (d[i]--) {
            n *= 10;
            n += i;
        }
    }
    return n;
}

int main (int argc, char *argv[])
{
    int n, v;
    u64 pdt, key;
    struct lnode *root = NULL;

    n = 100;
    while (1) {
        pdt = (u64)n * n * n;
        key = get_desc_sorted_number(pdt);
        v = add_to_sorted_tree(&root, key, n);
        if (v == 5) {
            printf("n = %d, n^3 = %I64u, key = %I64u\n", n, pdt, key);
            dump_sorted_tree_by_key(root, key);
            break;
        }
        n++;
    }
    //dump_sorted_tree(root);

    return 0;
}
