#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <error.h>
#include <errno.h>

struct node;

struct list {
	struct list *next, *prev;
};

struct edge {
	struct list list;

	struct node *node;

	int weight;
	int visited;
	int color;
};

struct node {
	struct list list;
	struct list edge_head;

	void *data;

	int index;
	int visited;
	int color;
};

struct graph {
	struct list node_head;
	int nr_nodes;
};

#define offset_of(type, member) (unsigned long)(&((type *)0)->member)
#define container_of(ptr, type, member)  \
        (type *)((unsigned long)(ptr) - offset_of(type, member))

#define for_each_node_in_list(lh, n) \
       for (n = (lh)->next; n != (lh); n = n->next)

#define generic_graph_iter(l1, l2, m1, T)               \
        for (l2 = (typeof(l2))(l1)->m1.next;            \
	     (l2 != (typeof(l2))&(l1)->m1) &&           \
	       (l2 = container_of(l2, T, list));        \
	     l2 = (typeof(l2))l2->list.next)

#define for_each_node_in_graph(g, n)                       \
        generic_graph_iter(g, n, node_head, struct node)

#define for_each_edge_in_node(n, e)                       \
        generic_graph_iter(n, e, edge_head, struct edge)

static void list_init(struct list *l)
{
	l->next = l->prev = l;
}

static void list_add_link(struct list *head, struct list *node)
{
	head->prev->next = node;
	node->next = head;
	node->prev = head->prev;
	head->prev = node;
}

static void graph_init(struct graph **graph, unsigned int nodes)
{
	int i;
	struct graph *g;
	struct node *n;

	g = calloc(1, sizeof(struct graph));
	if (!g) {
		perror("calloc");
		return;
	}

	*graph = g;
	list_init(&g->node_head);

	for (i = 0; i < nodes; i++) {
		n = calloc(1, sizeof(struct node));
		if (!n) {
			perror("calloc");
			return;
		}

		n->index = i+1;
		list_init(&n->list);
		list_init(&n->edge_head);

		list_add_link(&g->node_head, &n->list);
	}

	g->nr_nodes = nodes;
}

/* Connect: out -> in */
static void graph_connect_nodes(struct node *out, struct node *in)
{
	struct edge *edge;
	struct list *list;

	if (!in || !out || (in == out))
		return;

	/* check if nodes are already connected! */
	for_each_node_in_list(&out->edge_head, list) {
		edge = container_of(list, struct edge, list);

		if (edge->node == in)
			return;
	}

	edge = calloc(1, sizeof(struct edge));
	if (!edge) {
		perror("calloc");
		return;
	}

	edge->node = in;
	list_init(&edge->list);
	list_add_link(&out->edge_head, &edge->list);
}

static void graph_biconnect_nodes(struct node *n1, struct node *n2)
{
	if (n1 == n2)
		return;

	graph_connect_nodes(n1, n2);
	graph_connect_nodes(n2, n1);
}

static struct node *graph_find_node(struct graph *graph, int index)
{
	struct node *node;
	struct list *list;

	for_each_node_in_list(&graph->node_head, list) {
		node = container_of(list, struct node, list);

		if (node->index == index)
			return node;
	}

	return NULL;
}

static inline struct node *graph_get_first_node(struct graph *graph)
{
	return (graph->node_head.prev == &graph->node_head) ? NULL:
		container_of(graph->node_head.next, struct node, list);
}

static inline struct edge *node_get_first_edge(struct node *node)
{
	return (node->edge_head.prev == &node->edge_head) ? NULL:
		container_of(node->edge_head.next, struct edge, list);
}

static int graph_components(struct graph *graph)
{
	struct node *node, *node_q[1024], *node_vq[1024];
	struct edge *edge;
	int q_ptr = 0, vq_ptr = 0, nr_nodes = 0, comp = 0;

	for_each_node_in_graph(graph, node) {
		if (node->visited)
			continue;
		comp++;
		node_q[q_ptr++] = node;
		node->visited = comp;

		while (q_ptr) {
			node = node_q[--q_ptr];
			nr_nodes++;
			node_vq[vq_ptr++] = node;

			for_each_edge_in_node(node, edge) {
				if (edge->node->visited)
					continue;

				node_q[q_ptr++] = edge->node;
				edge->node->visited = comp;
			}
		}
	}

	while (vq_ptr) {
		node = node_vq[--vq_ptr];
		node->visited = 0;
	}

	return comp;
}

/* If a->b, then, is_nodes_connected(a,b) = 1, else, 0 */
static int is_nodes_connected(struct node *a, struct node *b)
{
	struct edge *edge;

	for_each_edge_in_node(a, edge) {
		if (edge->node == b)
			return 1;
	}

	return 0;
}

static int do_graph_dfs(struct node *node)
{
	struct edge *edge;

	if (node->visited)
		return 1;

	node->visited = 1;
	for_each_edge_in_node(node, edge) {
		if (!do_graph_dfs(edge->node))
			edge->node->visited = 0;
		else
			return 1;
	}

	return 0;
}

static int graph_is_acyclic(struct graph *graph)
{
	return !do_graph_dfs(graph_get_first_node(graph));
}

int main(int argc, char *argv[])
{
	int n, nidx, nedge;
	struct graph *g = NULL;
	struct node *in, *out;

	scanf("%d", &n);
	if (n <= 0)
		return -1;

	graph_init(&g, n);

	for (;;) {
		scanf("%d", &nidx);
		if (nidx < 0) break;

		out = graph_find_node(g, nidx);
		if (!out) continue;

		for (;;) {
			scanf("%d", &nedge);
			if (nedge < 0) break;

			in = graph_find_node(g, nedge);
			if (!in) continue;

			graph_connect_nodes(out, in);
		}
	}

	printf("Graph has %d connected component(s)\n", graph_components(g));
	printf("Graph %s cyclic!\n", (!graph_is_acyclic(g)) ? "is": "is not");

	return 0;
}
