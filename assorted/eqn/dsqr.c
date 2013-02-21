#include <stdio.h>
#include <malloc.h>

#include "list.h"

#define MAX_ELEMS    1024

static int get_digit_square(int n)
{
	int s = 0, k;

	while (n) {
		k = n % 10;
		s += k*k;
		n /= 10;
	}

	return s;
}

struct ptrlist { void *ptr; struct list list; };

int main(int argc, char *argv[])
{
	struct listhead cyhead;
	struct list frhead, *node;
	struct ptrlist frlist[MAX_ELEMS], *frnode;
	int i, s, arr[MAX_ELEMS];

	list_init(&frhead);
	for (i = 0; i < MAX_ELEMS; arr[i] = i+1, i++) {
		frlist[i].ptr = &arr[i];
		list_init(&frlist[i].list);
		list_init(&frlist[i].cylist);
		list_add(&frhead, &frlist[i].list);
	}

	/* dot output */
	i = 0;
	printf("digraph gr {\n");
	while (1) {
		arr[i] = 0;
		list_remove(&frlist[i].list);
		s = get_digit_square(i);
		printf("\t %d -> %d;\n", i, s);

		if (!arr[s]) {
			if (i == s) {
				node = malloc(sizeof(struct list));
				if (!node) {
					exit(-1);
				}
				list_init(cynode.list);
				list_add(&cyhead, cynode);
			}
			if (frhead.next == &frhead)
				break;
			frnode = container_of(struct ptrlist, frhead.next, list);
			i = *(int *)frnode->ptr-1;
		} else {
			i = s;
		}
	}
	printf("};\n");

	return 0;
}
