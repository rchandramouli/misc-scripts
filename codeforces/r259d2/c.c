#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[])
{
    int m, n, i;
    double exp, v, p;

    scanf("%d %d", &m, &n);
    exp = m;

    for (i = 1; i < m; i++) {
        v = (double)(m - i) / (double)m;
        p = pow(v, (double)n);
        exp -= p;
    }

    printf("%.12lf\n", exp);

    return 0;
}
