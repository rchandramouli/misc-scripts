#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* sum sequence: 1,3,13,61,277,1205,5077,20949 */

static int buf[8128], temp[8128], n, l;
static int odo[4096], max, lp;

#define copy_buf(x, y)  memcpy(x, y, l*sizeof(int))

#define __dump_check(x)					\
	do {						\
		printf("%s [%s]: ", __func__, x);	\
		for(i = 0; i < k; i++)			\
			printf("%d ", check[i]);	\
		printf("\n");				\
	} while (0)


static void odometer_init(void)
{
	int i;

	max = lp = 1;

	for (i = 0; i < l/2; i++)
		odo[i] = max;
}

static void odometer_dump(void)
{
	int i;

	for (i = 1; i < l/2; i++) {
		printf("%3d ", odo[i]);
	}
	printf("\n");
}

static void odometer_inc(void)
{
	int i, j;

	if (lp == l/2) {
		j = odo[lp-1];
		for (i = lp-2; i > 0; i--) {
			if (odo[i] > j) break;
		}
		lp = i+1;
		for (i = lp+1; i < l/2; i++)
			odo[i] = 1;
	}
	odo[lp]++;
	lp++;
}

static void test_odo(void)
{
	int i;

	odometer_init();
	for (i = 0; i < 64; i++) {
		odometer_dump();
		odometer_inc();
	}
}

static int check_folds(void)
{
	int i, j, k;
	int check[8128];

	copy_buf(check, temp);
	k = l;
	//__dump_check("initial");
	k = l/2;

	while (k > 0) {
		check[0] += check[2*k-1];
		j = check[0];

		for (i = 1; i < k; i++) {
			check[i] += check[2*k-i-1];
			if (check[i] <= j) return 0;
			j = check[i];
		}

		if (k == 1)
			__dump_check("fold");
		k /= 2;
	}

	return 1;
}

static void process(void)
{
	int i, j;

	odometer_init();

	while (1) {
		copy_buf(temp, buf);

		for (i = 1; i < l/2; i++) {
			for (j = i; j < l; j++)
				temp[j] += odo[i];

			if (check_folds()) {
				copy_buf(buf, temp);
				return;
			}
		}

		odometer_inc();
	}
}

static void dump(void)
{
	int i, j;

	printf("\n=========== DUMPING SEQUENCE ==========\n");
	for (i = j = 0; i < l; i++) {
		printf("%5d ", buf[i]);
		j++;
		if ((j % 12) == 0) { j = 0; printf("\n"); }
	}

	printf("\n=========== DUMPING ODOMETER ==========\n");
	for (i = 1, j = 0; i < l/2; i++) {
		printf("%+2d ", odo[i]);
		j++;
		if ((j % 13) == 0) { j = 0; printf("\n"); }
	}
	printf("\n\n");
}

static void fill(void)
{
	int i;

	for (i = 0; i < l; i++)
		buf[i] = i+1;
}

int main(int argc, char *argv[])
{
	scanf("%d", &n);
	if (n < 0 || n > 13) return -1;

	l = (1 << n);

	fill();
	process();
	dump();

	return 0;
}
