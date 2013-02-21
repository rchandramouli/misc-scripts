#include <stdio.h>


/* Apply rotation transformation:
 * 
 * pdt = 0: 1->1, 2->2, 3->3, 4->4
 * pdt = 1
 */
static int M[2][2] = { {1, 2}, {3, 4} };

int apply_rot_pdt (int r, int c, int pdt)
{
    int _r, _c;

    if (pdt == 0) {
        _r = r;
        _c = c;
        return M[_r-1][_c-1];

    } else if (pdt == 1) {
        _r = 3-r;
        _c = c;
        return M[_r-1][_c-1];

    } else if (pdt == 2) {
        _r = r;
        _c = 3-c;
        return M[_r-1][_c-1];

    } else if (pdt == 3) {
        _r = 3-r;
        _c = 3-c;
        return M[_r-1][_c-1];
    }
    return 0;
}

static int get_rot_cell (int k, int r, int c, int pdt)
{
    unsigned int v;

    if (k == 1) {
        return apply_rot_pdt(r, c, pdt);
    }

    v = 1 << (k-1);
    if (r <= v && c <= v) {
        return get_rot_cell(k-1, r, c, pdt);

    } else if (r <= v && c > v) {
        return get_rot_cell(k-1, r, c-v, pdt ^ 1);

    } else if (r > v && c <= v) {
        return get_rot_cell(k-1, r-v, c, pdt ^ 2);

    } else { /* r > v, c > v */
        return get_rot_cell(k-1, r-v, c-v, pdt ^3);
    }
}

int main (int argc, char *argv[])
{
    int k, r, c;

    scanf("%d", &k);

    for (r = 1; r <= (1 << k); r++) {
        for (c = 1; c <= (1 << k); c++) {
            printf("%3d ", get_rot_cell(k, r, c, 0));
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}
