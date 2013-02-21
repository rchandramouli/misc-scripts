/* Permutation polygons */
#include <stdio.h>
#include <math.h>

static int P[32], MaxP[32], N;
static double max_area;

static double poly_area (void)
{
    int i;
    int x1, y1, x2, y2;
    double area = 0;

    for (i = 1; i+1 < N; i++) {
        x1 = i;
        x2 = i+1;
        y1 = P[i] - P[0];
        y2 = P[i+1] - P[0];

        area += (double)(x1*y2 - x2*y1);
    }
    return fabs(area / 2.0);
}

static void dump_perm (int *p, int n)
{
    int i;

    printf("(");
    for (i = 0; i < n-1; i++) {
        printf("%d, ", p[i]);
    }
    printf("%d)\n", p[n-1]);
}

static void perm (int n)
{
    int i, t;

    if (n <= 1) {
        double area;

        area = poly_area();
        dump_perm(P, N);
        printf("\t\t\t area = %f\n", area);
        if (area > max_area) {
            max_area = area;
            for (i = 0; i < N; i++) {
                MaxP[i] = P[i];
            }
        }
        return;
    }

    for (i = 0; i < n; i++) {
        t = P[n-1];
        P[n-1] = P[i];
        P[i] = t;

        perm(n-1);

        t = P[n-1];
        P[n-1] = P[i];
        P[i] = t;
    }
}

int main (int argc, char *argv[])
{
    int i;

    N = 4;

    for (i = 0; i < N; i++) {
        P[i] = i+1;
    }

    perm(N);

    printf("\n\nMaximal area = %f\n", max_area);
    dump_perm(MaxP, N);
    
    return 0;
}
