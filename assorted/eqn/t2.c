#include <stdio.h>
#include <string.h>
#include <strings.h>

#define FMX_BASE0
#include "tloop.h"

#define MAX_COEFF   2402

static inline int last_state_1 (int state)
{
	return (state == 5);
}

static inline int next_state_1 (int prev, int val)
{
	int next = prev;

	switch (prev) {
	case 0:
		if (val == 2) next = 1;
		if (val == 1) next = 2;
		break;
	case 1: case 2:
		if (val == prev) next = 3;
		break;
	case 3: case 4:
		if (prev == val) next++;
		break;
	}
	return next;
}

static void dump_sequence (int *seq, int n)
{
	int i;

	printf("(%d", seq[0]);
	F(i,n,1) {
		printf(", %d", seq[i]);
	}
	printf(")\n");
}

static void dump_coeff (int *seq, int n, int mx)
{
	int i, j, k;

	F(i,n,0) {
		if (!seq[i]) continue;
		j = i;
		printf("<%d", j%7);
		F(k,mx,1) {
			j /= 7;
			printf(", %d", j%7);
		}
		printf(">  %3d\n", seq[i]);
	}
}

static int get_coeff (int *seq, int n)
{
	int i, c = 0;

	F(i,n,0) c = c*7 + seq[i];

	return c;
}

int main (int argc, char *argv[])
{
	int l[MX], cnt[MXVAL], c = 0, s, j;
	int coeff[MAX_COEFF];

	memset(coeff, 0, sizeof(int)*MAX_COEFF);
	FMX_N {
		s = 0;
		F(j,MX,0) {
			if (last_state_1(s = next_state_1(s, l[j]))) {
				c++;
				break;
			}
		}
		if (!last_state_1(s)) continue;

		/* Process for valid sequence */
		//dump_sequence(l, MX);
		memset(cnt, 0, sizeof(int)*MXVAL);
		F(j,MXVAL,0) cnt[j]=-1;
		F(j,MX,0) cnt[l[j]-1]++;
		s = get_coeff(cnt, MXVAL);
		//printf("\tCOEFF: %d\n", s);
		coeff[s]++;
	}
	dump_coeff(coeff, MAX_COEFF, MXVAL);
	printf("%d %d\n", MX, c);

	return 0;
}
