#ifndef LIST_H
#define LIST_H

struct list {
	struct list *next, *prev;
};

#define DECL_INIT_LIST(l)      struct list l = { .next = &l, .prev = &l }

static void inline list_head_init(struct list *list)
{
	list->next = list->prev = list;
}

static void inline list_add_node(struct list *head, struct list *node)
{
	node->next = head;
	node->prev = head->prev;
	head->prev->next = node;
	head->prev = node;
}

static void inline list_remove_node(struct list *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;
	node->next = node->prev = node;
}

#define offset_of(type, member) (unsigned long)(&((type *)0)->member)
#define container_of(ptr, type, member)  \
        (type *)((unsigned long)(ptr) - offset_of(type, member))

#define for_each_node(h, t)                           \
              for (t = (h)->next; t != (h); t = t->next)


#endif
