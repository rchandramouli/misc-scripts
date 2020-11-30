/* Find all solutions a^2*b^3 <= N, where a,b > 1 */
/* WIP */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

struct redux_table {
    long double div;
    long double pfx;
    uint64_t xt;
    uint64_t pw;
    uint64_t pdt;
};

static struct redux_table redux[] = {
    /* ..... */ { 4.0, 1.0/3.0, 2, 3, 1 },
    /* a=k^3 */ { 8.0, 1.0/6.0, 3, 6, 1 },
    /* a=k^4 */ { 8.0, 1.0/8.0, 3, 8, 1 },
    /* a=k^5 */ { 8.0, 1.0/10.0, 3, 10, 1 },
    /* a=k^6 */ { 8.0, 1.0/12.0, 3, 12, 1 },
    /* a=k^7 */ { 8.0, 1.0/14.0, 3, 14, 2 },
    /* a=k^8 */ { 8.0, 1.0/16.0, 3, 16, 2 },
    /* a=k^9 */ { 8.0, 1.0/18.0, 3, 18, 2 },
    /* b=k^3 */ { 4.0, 1.0/9.0, 2, 9, 1 },
    /* b=k^4 */ { 4.0, 1.0/12.0, 2, 12, 2 },
    /* b=k^5 */ { 4.0, 1.0/15.0, 2, 15, 2 },
    /* b=k^6 */ { 4.0, 1.0/18.0, 2, 18, 2 },
};

static int sz = sizeof(redux)/sizeof(redux[0]);

static uint64_t do_redux (struct redux_table *rd, uint64_t max_l)
{
    uint64_t i, j, nr = 0, pw;
    long double u, v;

    pw = 1+powl((long double)max_l/rd->div, rd->pfx);

    for (i = 2; i <= pw; i++) {
	j = powl(i, rd->pw);
	u = (long double)max_l / (long double)j;
	v = powl(u, (long double)1.0/(long double)rd->xt);
	nr += v-1;
	if (j*powl(v+1, rd->xt) <= max_l)
	    nr++;
    }

    printf("%s: (%Lf, %Lf, %"PRIu64", %"PRIu64", %"PRIu64") "
	   "max_l: %"PRIu64", pw: %"PRIu64", nr: %"PRIu64"\n", __func__,
	   rd->div, rd->pfx, rd->xt, rd->pw, rd->pdt, max_l, pw, nr);

    return nr;
}

int main (int argc, char *argv[])
{
    uint64_t NR = 0, max_l;
    int i;

    scanf("%"SCNu64, &max_l);

    NR += do_redux(&redux[0], max_l);
    for (i = 1; i < sz; i++) {
	NR -= redux[i].pdt * do_redux(&redux[i], max_l);
    }

    printf("COUNT: %"PRIu64"\n", NR);

    return 0;
}
