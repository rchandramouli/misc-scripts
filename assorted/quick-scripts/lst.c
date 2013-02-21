#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct list {
	int val;
	struct list *next;
};

static void list_dump(struct list *l)
{
	while (l != NULL) {
		printf("%d ", l->val);
		l = l->next;
	}
	printf("\n");
}

static void cyclic_list_dump(struct list *h)
{
	struct list *n;

	n = h;
	do {
		printf("%d ", n->val);
		n = n->next;
	} while (n != h);
	printf("\n");
}

static struct list *list_reverse(struct list *f, struct list *r)
{
	if (f == NULL || f->next == NULL)
		return f;

	r = list_reverse(f->next, r);
	f->next->next = f;
	f->next = NULL;

	return r;
}

static struct list *cyclic_list_reverse(struct list *f, struct list *l)
{
	if (l == NULL || l == f || l->next == f)
		return l;

	f = cyclic_list_reverse(f, l->next);
	l->next->next = l;
	l->next = f;

	return f;
}

static struct list *build_list(int n, int circ)
{
	struct list *l;
	int i;

	l = malloc(sizeof(struct list)*n);
	if (!l) return NULL;

	for (i = 0; i < n-1; i++) {
		l[i].val = i;
		l[i].next = &l[i+1];
	}
	l[i].val = i;
	l[i].next = (circ) ? &l[0]: NULL;

	return l;
}

static void list_delete(struct list *l)
{
	/* Hah, assuming l is just an array allocated! */
	free(l);
}

int main(int argc, char *argv[])
{
	int i;
	struct list *head, *rev;

	/* single ll */
	head = build_list(10, 0);
	list_dump(head);
	rev = list_reverse(head, NULL);
	list_dump(rev);
	list_delete(head);

	/* circular single ll */
	head = build_list(12, 1);
	cyclic_list_dump(head);
	rev = cyclic_list_reverse(head, head);
	cyclic_list_dump(rev);
	list_delete(head);

	return 0;
}
