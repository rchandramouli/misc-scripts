/* Find all solutions a^2*b^3 <= N, where a,b > 1 */
/* WIP */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

#define MAX_BUF_SZ     (30000001)
int ARR[MAX_BUF_SZ];

int main (int argc, char *argv[])
{
    uint64_t nr, i, j, m, mx3, max_l, xtra, pdt;
    uint64_t pw2, pw3, pw6, k, l, t;
    long double x3 = 1.0/3.0, x6 = 1.0/6.0, u, v;

    scanf("%"SCNu64, &max_l);

    mx3 = 1+powl((long double)max_l/4.0, x3);

    pw2 = 1+sqrtl((long double)max_l/512.0);
    pw3 = 1+powl((long double)max_l/256.0, x3);
    pw6 = 1+powl((long double)max_l/32.0, x6);

    printf("MX3 = %"PRIu64", PW: (%"PRIu64", %"PRIu64", %"PRIu64") -- max-l: %"PRIu64"\n",
	   mx3, pw2, pw3, pw6, max_l);

    memset(ARR, 0, sizeof(ARR));

    nr = 0;
    for (i = 2; i <= mx3; i++) {
	j = i*i*i;
	u = (long double)max_l / (long double)j;
	v = sqrtl(u);
	nr += v-1;
	printf("---> i=%"PRIu64", nr = %"PRIu64"  (MX: %"PRIu64", %"PRIu64")\n",
	       i, nr, j*((uint64_t)v*(uint64_t)v), (uint64_t)v);

	for (m = 2; m <= v; m++) {
	    if (j*m*m <= max_l)
		ARR[j*m*m]++;
	}
    }

    xtra = 0;
    uint64_t prev_xtra = 0;
    for (t = 2; t <= pw6; t++) {
	for (k = 2; k <= pw3; k++) {
	    for (l = 2; l <= pw2; l++) {
		pdt = k*k*k*l*l*t*t*t*t*t*t;
		if (pdt > max_l) {
		    if (prev_xtra) {
			printf("===> MAX: (%"PRIu64", %"PRIu64", %"PRIu64") XT: %"PRIu64"\n",
			       l, k, t, prev_xtra);
			xtra += prev_xtra;
			prev_xtra = 0;
		    }
		    break;
		}
		prev_xtra++;
	    }
	}
    }

    printf("Total: %"PRIu64", 2-xtra: %"PRIu64"\n", nr-xtra, xtra);

    nr = xtra = 0;
    for (i = 0; i < max_l; i++) {
	if (ARR[i] >= 2) {
	    printf("    V = %"PRIu64", nr = %d\n", i, ARR[i]);
	    nr++;
	} else if (ARR[i] == 1) {
	    xtra++;
	}
    }

    printf("Check: 2-xtra: %"PRIu64", other-xtra: %"PRIu64"\n", xtra, nr);

    return 0;
}
