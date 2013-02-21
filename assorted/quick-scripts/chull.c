#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <time.h>

struct list {
    struct list *next, *prev;
};

static inline void list_init (struct list *head)
{
    head->next = head->prev = head;
}

static inline void list_remove (struct list *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node->next = node;
}

static inline void list_add (struct list *head, struct list *node)
{
    node->prev = head;
    node->next = head->prev;
    head->prev->next = node;
    head->prev = node;
}

struct point {
    struct list ordlist;
    struct list hlist;
    int x, y;
    int hull;
};

#define MAX_PTS    1024

static struct list hull_heads, ord_head;
static struct point points[MAX_PTS];
static int npts;

static void sort_points (int n, struct point *pts, struct list *ord)
{
    int i, j, v;

    list_init(ord);

    for (i = 0; i < n; i++) {
	for (j = i; j < n; j++) {
	    if (pts[i].x > pts[j].x) {
		v = pts[i].x;
		pts[i].x = pts[j].x;
		pts[j].x = v;

		v = pts[i].y;
		pts[i].y = pts[j].y;
		pts[j].y = v;
	    } else if ((pts[i].x == pts[j].x) &&
		       (pts[i].y > pts[j].y)) {
		v = pts[i].y;
		pts[i].y = pts[j].y;
		pts[j].y = v;
	    }
	}
    }

    /* Queue all points in the list */
    for (i = 0; i < n; i++) {
	list_init(&pts[i].ordlist);
	list_init(&pts[i].hlist);
	pts[i].hull = 0;
	list_add(ord, &pts[i].ordlist);
    }
}

#define offset_of(T, m)         (&((T *)0)->m)
#define container_of(p, T, m)   ((T *)(((unsigned long)p)-offset(T, m)))
#define ord2pt(l)               container_of(l, struct point, ordlist)
#define hlist2pt(l)             container_of(l, struct point, hlist)

static inline int is_left (struct point *p1, struct point *p2, struct point *p3)
{
    return (((p3->y-p1->y)*(p2->x-p1->x)) <= ((p3->x-p1->x)*(p2->y-p1->y)));
}

static struct list *get_convex_hull_ring (struct list *ord)
{
    struct list uhlist, lhlist, *node, *hlist, *iter;
    struct point *p1, *p2, *p3;

    /* Empty list */
    if (ord->next == ord->prev && ord->next == ord)
	return NULL;

    hlist = calloc(sizeof(struct list), 1);
    if (!hlist) return NULL;

    list_init(hlist);

    /* If the set has less than four points, add them up */
    if (ord->next->next == ord || ord->next->next->next == ord ||
	ord->next->next->next->next == ord) {
	iter = ord->next;

	while (iter != ord) {
	    p1 = ord2pt(iter);
	    list_add(hlist, &p1->hlist);
	    iter = iter->next;
	    list_delete(&p1->ordlist);
	}
	return hlist;
    }

    list_init(&uhlist);
    list_init(&lhlist);

    /* Upper hull */
    p1 = ord2pt(ord->next);
    list_add(&uhlist, &p1->hlist);
    p1 = ord2pt(ord->next->next);
    list_add(&uhlist, &p1->hlist);
    node = &p1->ordlist.next;
    iter = uhlist.prev;

    while (node != ord) {
	p3 = ord2pt(node);

	while (iter->prev != &uhlist) {
	    p1 = ord2pt(iter->prev);
	    p2 = ord2pt(iter);

	    iter = iter->prev;
	    if (is_left(p1, p2, p3)) {
		list_del(&uhlist, &p2->hlist);
	    }
	}
	list_add(&uhlist, &p3->hlist);
	node = node->next;
    }

    /* Lower hull */
    p1 = ord2pt(ord->prev);
    list_add(&lhlist, &p1->hlist);
    

    return hlist;
}

static void generate_points (int n, struct point *pts)
{
    int i;

    for (i = 0; i < n; i++) {
	list_init(&pts[i].hlist);
	pts[i].hull = 0;
	pts[i].x = random() & 0x1f;
	pts[i].y = random() & 0x1f;
    }
}

int main (int argc, char *argv[])
{
    struct list *hull;

    srandom(time(NULL));

    scanf("%d", &npts);
    if (npts <= 2 || npts >= MAX_PTS) return -1;

    generate_points(npts, points);
    sort_points(npts, points, &ord_head);

    list_init(&hull_heads);

    while (1) {
	hull = get_convex_hull_ring(&ord_head);
	if (!hull) break;

	list_add(&hull_heads, hull);
    }

    return 0;
}
