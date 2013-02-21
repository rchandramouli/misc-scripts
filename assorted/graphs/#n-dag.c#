/* Given a transitive tournament on n nodes, produce an edge relaxed
 * DAG by addition of virtual nodes.
 */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define NODE_STR     "  %c%d"
#define EDGE_STR     "  %c%d -> %c%d;\n"
#define abs(n)       (((n)>=0)?(n):(-(n)))
#define _(n)         (((n)>=0)?'.':'V'), abs(n)

static inline void dag_n4(int n1, int n2, int n3, int n4)
{
	printf(EDGE_STR EDGE_STR EDGE_STR EDGE_STR EDGE_STR EDGE_STR,
	       _(n1), _(n2), _(n1), _(n3), _(n1), _(n4),
	       _(n2), _(n3), _(n2), _(n4), _(n3), _(n4));
}
static inline void dag_n3(int n1, int n2, int n3)
{
	printf(EDGE_STR EDGE_STR EDGE_STR,
	       _(n1), _(n2), _(n1), _(n3), _(n2), _(n3));
}
static inline void dag_n2(int n1, int n2)
{
	printf(EDGE_STR, _(n1), _(n2));
}

int main(int argc, char *argv[])
{
	int i, n, c, ni, v;
	int *nodes;

	scanf("%d", &n);
	if (n <= 1) return -1;

	nodes = calloc(sizeof(int), n+(n/2));
	if (!nodes) return -1;

	ni = c = v = 1;
	i = 0;
	while (ni <= n) {
		nodes[i] = ni;
		ni++;
		c++;
		i++;
		if (c == 4 && ni < n) {
			nodes[i] = -v;
			v++;
			i++;
			c = 2;
		}
	}

	/*
	  for (c = 0; c < i; c++)
	  printf("%c%d ", _(nodes[c]));
	  printf("\n\n");
	*/
	printf("digraph g {\nnode [label=\"\",fixedsize=true,shape=\"circle\","
	       "style=\"bold,filled\",fontsize=0,color=\"#000000\","
		"width=.1,height=.1];\n"
	       "edge [arrowsize=.6,color=\"#888888\"];\n\n");

	for (c = 0; c < i; c++) {
		if (nodes[c] < 0) {
			printf(NODE_STR "[color=\"red\"];\n", _(nodes[c]));
		}
	}
	printf("\n");
	for (c = 0, ni = 0; c < i; c++) {
		if (nodes[c] > 0 && c != i-1)
			continue;

		switch (c-ni) {
		case 1:
			dag_n2(nodes[ni], nodes[ni+1]);
			break;
		case 2:
			dag_n3(nodes[ni], nodes[ni+1], nodes[ni+2]);
			break;
		case 3:
			dag_n4(nodes[ni], nodes[ni+1],
			       nodes[ni+2], nodes[ni+3]);
			break;
		}
		ni = c;
	}
	printf("};\n");

	return 0;
}
