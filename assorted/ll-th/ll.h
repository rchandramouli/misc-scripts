/* Lockless linked list implementation */
#ifndef LL_H
#define LL_H

struct list {
	struct list *prev, *next;
};

#define offset_of(type, member) (unsigned long)(&((type *)0)->member)
#define container_of(ptr, type, member)  \
        (type *)((unsigned long)(ptr) - offset_of(type, member))

#define for_each_node_in_list(lh, n) \
       for (n = (lh)->next; n != (lh); n = n->next)

#define for_each_node_in_list_safe(lh, n, t) \
       for (n = (lh)->next, t = n->next; n != (lh); n = t, t = t->next)

/* init */
static inline void list_init(struct list *node)
{
	node->next = node->prev = node;
}

/* add */
static inline void list_add_link(struct list *head, struct list *node)
{
	node->prev = head->prev;
	node->next = head;
	head->prev->next = node;
	head->prev = node;
}

/* remove */
static inline void list_remove_link(struct list *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node->next = node;
}

/* search */
static __attribute__((unused))
struct list *list_find_node(struct list *head, struct list *key,
				   int (*fcmp)(struct list *l1, struct list *l2))
{
	struct list *node;

	for_each_node_in_list(head, node) {
		if (!(*fcmp)(node, key))
			return node;
	}

	return NULL;
}

#endif /* LL_H */
