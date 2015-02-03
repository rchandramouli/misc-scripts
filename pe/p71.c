#include <stdio.h>
#include <malloc.h>

typedef unsigned long long int u64;

struct lnode;
struct lnode {
    struct lnode *left, *right;
};

struct sorted_tree {
    u64 n;
    u64 d;

    struct lnode link;
};

#define offset_of(T, m)  ((unsigned long)(&((T *)0)->m))

static inline struct sorted_tree *get_sorted_tree_obj (struct lnode *node)
{
    if (node == NULL) return NULL;

    return (struct sorted_tree *)(((unsigned long)node) -
                                  offset_of(struct sorted_tree, link));
}

static inline void init_sorted_tree (struct sorted_tree *s, u64 n, u64 d)
{
    s->n = n;
    s->d = d;
    s->link.left = s->link.right = NULL;
}

static inline void add_node_to_node (struct sorted_tree *root,
                                     struct sorted_tree *node,
                                     int is_left)
{
    if (is_left) {
        root->link.left = &node->link;
    } else {
        root->link.right = &node->link;
    }
}


static inline void add_to_sorted_tree (struct lnode **root, u64 n, u64 d)
{
    struct lnode *temp = *root;
    struct sorted_tree *node, *iter;
    int is_left = 0;

    if (*root == NULL) {
        node = malloc(sizeof(struct sorted_tree));
        *root = &node->link;

        init_sorted_tree(node, n, d);
        return;
    }
    while (1) {
        iter = get_sorted_tree_obj(temp);

        if (iter->n*d > iter->d*n) {
            is_left = 1;
            temp = temp->left;
        } else if (iter->n*d < iter->d*n) {
            is_left = 0;
            temp = temp->right;
        } else {
            /* Element exists, return */
            return;
        }
        if (temp == NULL) {
            node = malloc(sizeof(struct sorted_tree));
            init_sorted_tree(node, n, d);
            add_node_to_node(iter, node, is_left);
            break;
        }
    }
}

static int elem_count = 0;
static inline void dump_sorted_tree_node (struct sorted_tree *s)
{
    if (elem_count == 20)
        printf("\n");

    printf("%I64u/%I64u ", s->n, s->d);
    elem_count = (elem_count + 1) % 21;
}
    
static void dump_sorted_tree (struct lnode *root)
{
    if (!root)
        return;

    if (root->left)
        dump_sorted_tree(root->left);

    dump_sorted_tree_node(get_sorted_tree_obj(root));

    if (root->right)
        dump_sorted_tree(root->right);
}

static struct lnode *get_inorder_pred (struct lnode *root, u64 n, u64 d)
{
    struct lnode *node = root;
    struct sorted_tree *obj;
    int found = 0;

    while (node) {
        obj = get_sorted_tree_obj(node);
        if (obj->n == n && obj->d == d) {
            found = 1;
            break;
        }
        if (obj->n*d > obj->d*n) {
            node = node->left;
        } else if (obj->n*d < obj->d*n) {
            node = node->right;
        }
    }

    if (found) {
        node = node->left;
        if (node) {
            while (node->right)
                node = node->right;
        }
    }

    return node;
}

static u64 get_gcd (u64 a, u64 b)
{
    if (a > b)
        return get_gcd(b, a);

    if (a == 0)
        return b;

    return get_gcd(b % a, a);
}

int main (int argc, char *argv[])
{
    u64 n, d, pn;
    int k;
    struct lnode *root = NULL;
    struct sorted_tree *temp;

    scanf("%d", &k);

#define N  (u64)42857
#define D  (u64)100000

    for (d = 2; d <= (u64)k; d++) {
        for (n = 1, pn = 0; n < d; n++) {
            if ((u64)7*n > (u64)3*d) break;
            if (D*n < N*d) {
                n = N*d;
                n /= D;
                if (pn && pn <= n) {
                    n++;
                }
                pn = n;
                continue;
            }
            if ((d % n) == 0) continue;

            if (get_gcd(n, d) == 1) {
                add_to_sorted_tree(&root, n, d);
            }
        }
    }

    //dump_sorted_tree(root);
    temp = get_sorted_tree_obj(get_inorder_pred(root, 3, 7));

    if (temp) {
        printf("\nPREV(3, 7) elem = %I64u / %I64u\n", temp->n, temp->d);
    }

    return 0;
}
