#ifndef __LIST_H__
#define __LIST_H__

struct list { struct list *next, *prev; };

static inline void list_init(struct list *l)
{
	l->next = l->prev = l;
}

static inline void list_remove(struct list *l)
{
	l->next->prev = l->prev;
	l->prev->next = l->next;
	list_init(l);
}

static inline void list_add(struct list *h, struct list *n)
{
	n->next = h;
	n->prev = h->prev;
	h->prev->next = n;
	h->prev = n;
}

struct listhead { struct list head; struct list list; };

struct inline void list_head_init(struct listhead *lh)
{
	list_init(&lh->list);
	list_init(&lh->head);
}

#define offset_of(s, m)              ((unsigned long)&(((s *)0)->m))
#define container_of(s, p, m)        ((s *)((unsigned long)p - offset_of(s, m)))

#define for_each_node_safe(h, n, t)		\
	for (t = h, n = t->next; n != h; t = n, n = t->next)
#define for_each_node(h, n)			\
	for (n = h->next; n != h; n = n->next)

#endif /* __LIST_H__ */
