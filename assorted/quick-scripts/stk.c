#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <errno.h>

#include <unistd.h>
#include <time.h>

#define STK_ERR_INVAL            1
#define STK_ERR_EMPTY            2
#define STK_ERR_FULL             3
#define STK_ERR_UNINITIALIZED    4
#define STK_ERR_ARG_NULL         5
#define STK_ERR_UNKNOWN          6

/* Assuming we do not get allocations within the first 4K page */
#define ERR_TO_PTR(x)     ((void *)((x) % 4096))
#define IS_ERR(x)         (((unsigned long)(x) < 4096) && ((unsigned long)x != 0))
#define PTR_TO_ERR(x)     ((unsigned long)(x))

#define offset_of(type, member) (unsigned long)(&((type *)0)->member)
#define container_of(ptr, type, member)  \
        (type *)((unsigned long)(ptr) - offset_of(type, member))

#define debug(x, f, ...)       printf("%s: " x f, __func__, __VA_ARGS__)
#define err_debug(x, ...)   debug("[error] ", x, __VA_ARGS__)
#define info_debug(x, ...)  debug("[info] ", x, __VA_ARGS__)

#define fetch_stack_node(l, type)                                \
({                                                               \
	struct list_node *t = (l)->type;                         \
        struct stack_node *stk = NULL;                           \
        if (t != NULL)                                           \
                stk = container_of(t, struct stack_node, list);  \
        stk;                                                     \
})

#define next_stack_node(stk) fetch_stack_node(&((stk)->list), next)
#define prev_stack_node(stk) fetch_stack_node(&((stk)->list), prev)

#define for_each_node_in_list(lh, n, t) \
       for (n = (lh), t = n->next; t != n; n = t, t = n->next)

#define list_to_stack_node(l) container_of(l, struct stack_node, list)

struct list_node {
	struct list_node *next, *prev;
};

struct stack_node {
	void *object;
	struct list_node list;
};

struct stack {
	struct stack_node *empty;
	struct stack_node *active;

	struct stack_node *head;

	int nr_used;
	int nr_empty, nr_active;
};

static inline void
list_split_heads(struct list_node *head1, struct list_node *head2)
{
	
}

static inline void
list_unlink_node(struct list_node *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node->next = node;
}

static inline void
list_add_to_head(struct list_node *head, struct list_node *node)
{
	node->prev = head->prev;
	node->next = head;
	node->prev->next = node;
	head->prev = node;
}

static inline void
list_head_append_list(struct list_node *hf, struct list_node *hl)
{
	hf->prev->next = hl->prev;
	hl->prev = hf->prev;
	hf->prev = hf->prev->next;
	hl->prev->next = hl;
	hf->prev->next = hf;
}

/* FIXME: Implement */
#define adjust_active_list(x)   (1)

static int
allocate_n_node_list(struct stack_node **root, int count)
{
	struct stack_node *node;
	int i;

	for (i = 0; i < count; i++) {
		node = malloc(sizeof(struct stack_node));
		if (node == NULL)
			goto fail_alloc;

		node->object = NULL;
		node->list.prev = node->list.next = &node->list;

		if (i) {
			list_add_to_head(&(*root)->list, &node->list);
		} else {
			*root = node;
		}
	}

 fail_alloc:
	return i;
}

/* Grow the stack by adding nr_nodes more elements */
int
stack_grow(struct stack *stk, int nr_nodes)
{
	int count;
	struct stack_node *node;

	if (stk == NULL)
		return STK_ERR_ARG_NULL;

	if (nr_nodes <= 0)
		return STK_ERR_INVAL;

	count = allocate_n_node_list(&node, nr_nodes);
	if (count) {
		stk->nr_empty += count;
		if (stk->empty == NULL)
			stk->empty = node;
		else {
			list_head_append_list(&stk->empty->list, &node->list);
		}
	}

	return count;
}

/* Push into the stack */
int
stack_push(struct stack *stk, void *object)
{
	if (stk == NULL)
		return STK_ERR_ARG_NULL;

	if (stk->active == NULL) {
		int err;

		if (stk->empty == NULL)
			return STK_ERR_UNINITIALIZED;

		if ((err = adjust_active_list(stk)))
			return err;
	}

	if (stk->nr_used == stk->nr_active)
		return STK_ERR_FULL;

	if (stk->head == NULL)
		stk->head = stk->active;

	/* Push to the head and move along */
	stk->head->object = object;
	stk->head = next_stack_node(stk->head);
	stk->nr_used++;

	return 0;
}

void *
stack_pop(struct stack *stk)
{
	struct stack_node *n;
	void *object;

	if (stk == NULL)
		return ERR_TO_PTR(STK_ERR_ARG_NULL);

	if (stk->active == NULL || stk->nr_used == 0)
		return ERR_TO_PTR(STK_ERR_EMPTY);

	stk->nr_used--;
	if (stk->head == stk->active && stk->nr_used > 0) {
		n = prev_stack_node(stk->active);
		stk->head = n;
	}

	n = stk->head;
	stk->head = (stk->nr_used) ? prev_stack_node(n): NULL;
	object = n->object;
	n->object = NULL;

	return object;
}

static void
stack_list_fini(struct stack_node **head, void (*object_fini)(void *))
{
	struct list_node *node, *temp;
	struct stack_node *stk_node;

	for_each_node_in_list(&(*head)->list, node, temp) {
		stk_node = list_to_stack_node(node);

		list_unlink_node(&stk_node->list);
		if (stk_node->object && object_fini) {
			object_fini(stk_node->object);
		}
		free(stk_node);
	}
	free(list_to_stack_node(node));
	*head = NULL;
}

void
stack_fini(struct stack *stk, void (*object_fini)(void *))
{
	if (stk == NULL)
		return;

	if (stk->active) {
		stack_list_fini(&stk->active, object_fini);
		stk->nr_active = stk->nr_used = 0;
	}

	if (stk->empty) {
		stack_list_fini(&stk->empty, object_fini);
		stk->nr_empty = 0;
	}
}

/* Initialize the stack */
struct stack *
stack_init(int *nr_nodes)
{
	struct stack *stk;
	struct stack_node *node, *root;
	int count, i, err = 0;

	if (nr_nodes == NULL)
		return ERR_TO_PTR(STK_ERR_ARG_NULL);

	if (*nr_nodes <= 0)
		return ERR_TO_PTR(STK_ERR_INVAL);

	stk = malloc(sizeof(struct stack));
	if (stk == NULL)
		return ERR_TO_PTR(errno);

	stk->head = NULL;
	stk->empty = NULL;
	stk->active = NULL;

	count = allocate_n_node_list(&root, *nr_nodes);

 fail_alloc:
	if (!count) {
		free(stk);
	} else {
		stk->active = stk->head = root;
		stk->nr_used = stk->nr_empty = 0;
		stk->nr_active = count;

		*nr_nodes = count;
	}

	return (err) ? ERR_TO_PTR(err): stk;
}

#define STACK_MAX_NODES    1024

static int elems[STACK_MAX_NODES];

struct test_result {
	/* pass, expected pass, fail, expected fail */
	int p;
	int ep;
	int f;
	int ef;
};

/* Test initialization */
void test_stack_init(struct test_result *tr)
{
	struct stack *stk;
	int n = 0;

	tr->ef++;
	stk = stack_init(NULL);
	if (IS_ERR(stk) && (PTR_TO_ERR(stk) == STK_ERR_ARG_NULL))
		tr->f++;

	tr->ef++;
	n = 0;
	stk = stack_init(&n);
	if (IS_ERR(stk) && (PTR_TO_ERR(stk) == STK_ERR_INVAL))
		tr->f++;

	tr->ep++;
	n = 1;
	stk = stack_init(&n);
	if (!IS_ERR(stk)) {
		tr->p++;
		stack_fini(stk, NULL);
	}
}

void test_stack_push(struct test_result *tr)
{
	struct stack *stk;
	int err, n, init = 0;

	tr->ef++;
	err = stack_push(NULL, NULL);
	if (err == STK_ERR_ARG_NULL)
		tr->f++;

	tr->ep++;
	n = 1;
	stk = stack_init(&n);
	if (!IS_ERR(stk)) {
		init = 1;
		err = stack_push(stk, &n);
		if (!err) {
			tr->p++;
		}
	}

	tr->ef++;
	err = stack_push(stk, &n);
	if (err == STK_ERR_FULL)
		tr->f++;

	if (init) {
		stack_fini(stk, NULL);
	}
}

void test_stack_pop(struct test_result *tr)
{
	struct stack *stk;
	int err, n, *p, init = 0;

	tr->ef++;
	p = stack_pop(NULL);
	if (IS_ERR(p) && (PTR_TO_ERR(p) == STK_ERR_ARG_NULL))
		tr->f++;

	tr->ep++;
	n = 1;
	stk = stack_init(&n);
	if (!IS_ERR(stk)) {
		init = 1;
		err = stack_push(stk, &n);
		if (!err) {
			p = stack_pop(stk);
			if (!IS_ERR(p) && (p == &n))
				tr->p++;
		}
	}

	tr->ef++;
	p = stack_pop(stk);
	if (IS_ERR(p) && (PTR_TO_ERR(p) == STK_ERR_EMPTY))
		tr->f++;

	if (init) {
		stack_fini(stk, NULL);
	}
}

void test_stack_push_pop(struct test_result *tr)
{
	struct stack *stk;
	int err, n, *p, init = 0, rep = 2;

	n = 1;
	stk = stack_init(&n);
	if (!IS_ERR(stk)) {
		init = 1;

		while (rep--) {
			err = stack_push(stk, &n);
			if (err) {
				continue;
			}

			tr->ef++;
			err = stack_push(stk, &n);
			if (err == STK_ERR_FULL)
				tr->f++;

			tr->ep++;
			p = stack_pop(stk);
			if (!IS_ERR(p) && (p == &n))
				tr->p++;

			tr->ef++;
			p = stack_pop(stk);
			if (IS_ERR(p) && (PTR_TO_ERR(p) == STK_ERR_EMPTY))
				tr->f++;
		}
	}

	if (init) {
		stack_fini(stk, NULL);
	}
}

void do_run_test(char *name, void (*fn)(struct test_result *),
		 struct test_result *result)
{
	struct test_result tr;

	if (!fn) {
		return;
	}

	tr.p = tr.ep = tr.f = tr.ef = 0;
	fn(&tr);
	printf("%s: P # %d/%d, F # %d/%d\n", name, tr.p, tr.ep, tr.f, tr.ef);

	result->p += tr.p;
	result->ep += tr.ep;
	result->f += tr.f;
	result->ef += tr.ef;
}

#define perform_test(fn, tr)      do_run_test(#fn, fn, tr)

int main(int argc, char *argv[])
{
	struct test_result tr;

	tr.p = tr.ep = tr.f = tr.ef = 0;
	
	perform_test(test_stack_init, &tr);
	perform_test(test_stack_push, &tr);
	perform_test(test_stack_pop, &tr);
	perform_test(test_stack_push_pop, &tr);

	printf("\nTotal counts: P # %d/%d, F # %d/%d\n",
	       tr.p, tr.ep, tr.f, tr.ef);

	return 0;
}
