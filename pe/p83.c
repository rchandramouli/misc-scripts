#include <stdio.h>

#define MAX_X   100
#define MAX_Y   100

#define INF 1000000000

static int max_x, max_y;

static int visits[MAX_X][MAX_Y];
static int cost[MAX_X][MAX_Y];
static int v_cost[MAX_X][MAX_Y];

static inline void mark_visited (int x, int y)
{
    visits[x][y] = 1;
}

static inline void clear_visited (int x, int y)
{
    visits[x][y] = 0;
}

static inline int visited (int x, int y)
{
    return visits[x][y];
}

static int find_path_sum (int x, int y)
{
    int l[4], m, i, idx;

    if (v_cost[x][y] != INF)
        return v_cost[x][y];

    mark_visited(x, y);

    l[0] = INF;
    if (x > 0 && !visited(x-1, y))
        l[0] = find_path_sum(x-1, y);

    l[1] = INF;
    if (y > 0 && !visited(x, y-1))
        l[1] = find_path_sum(x, y-1);

    l[2] = INF;
    if (x < max_x-1 && !visited(x+1, y))
        l[2] = find_path_sum(x+1, y);

    l[3] = INF;
    if (y < max_y-1 && !visited(x, y+1))
        l[3] = find_path_sum(x, y+1);

    clear_visited(x, y);

    m = INF;
    for (i = 0; i < 4; i++) {
        if (l[i] != INF)
            if (l[i] < m) {
                m = l[i];
                idx = i;
            }
    }
    if (m != INF) {
        m += cost[x][y];
    }
    if (v_cost[x][y] > m)
        v_cost[x][y] = m;

    return v_cost[x][y];
}

static int retrim_matrix (void)
{
    int m, x, y;
    int found = 0;

    for (x = 0; x < max_x; x++) {
        for (y = 0; y < max_y; y++) {
            m = INF;
            if (x > 0)
                m = v_cost[x-1][y];
            if (y > 0)
                if (m > v_cost[x][y-1])
                    m = v_cost[x][y-1];
            if (x < max_x-1)
                if (m > v_cost[x+1][y])
                    m = v_cost[x+1][y];
            if (y < max_y-1)
                if (m > v_cost[x][y+1])
                    m = v_cost[x][y+1];
            if (m != INF &&
                v_cost[x][y] > (cost[x][y] + m)) {
                found = 1;
                v_cost[x][y] = cost[x][y] + m;
            }
        }
    }
    return found;
}

static void dump_v_cost (void)
{
    int i, j;

    for (i = 0; i < max_x; i++) {
        for (j = 0; j < max_y; j++) {
            printf("%4d ", v_cost[i][j]);
        }
        printf("\n");
    }
}

int main (int argc, char *argv[])
{
    int i, j;
    int c = 500;

    scanf("%d %d", &max_x, &max_y);

    for (i = 0; i < max_x; i++) {
        for (j = 0; j < max_y; j++) {
            scanf("%d", &cost[i][j]);
            v_cost[i][j] = INF;
        }
    }
    v_cost[max_x-1][max_y-1] = cost[max_x-1][max_y-1];

    printf("finding low cost routes from each node ... \n");
    find_path_sum(0, 0);

    while (c-- && retrim_matrix()) {
        //dump_v_cost();
        //printf("\n===============================\n");
    }

    printf("%d (retrim = %d)\n", v_cost[0][0], 500-c);

    return 0;
}
