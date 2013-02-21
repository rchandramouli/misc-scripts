#include <stdio.h>
#include <malloc.h>

static int P, **ptable, *coeff;

static void input_poly_coeffs(void)
{
	int i;

	for (i = 0; i < P; i++) {
		scanf("%d", &coeff[i]);
		coeff[i] %= P;
	}
}

/* compute: pz = px * py */
static void get_poly_product(int *px, int *py, int *pz)
{
	int i, j, pwr;

	/* naive, O(n^2) */
	for (i = 0; i < P; i++)
		pz[i] = (px[0] * py[i]) % P;

	for (i = 1; i < P; i++) {
		if (!px[i]) continue;
		for (j = 0; j < P; j++) {
			if (!py[j]) continue;
			pwr = (j+i) % (P-1);
			pwr = (!pwr) ? P-1: pwr;

			pz[pwr] = (pz[pwr] + px[i]*py[j]) % P;
		}
	}
}

static void compute_poly_power_matrix(void)
{
	int i;

	ptable[0][0] = 1;
	ptable[1][0] = coeff[0];
	for (i = 1; i < P; i++) {
		ptable[0][i] = 0;
		ptable[1][i] = coeff[i];
	}

	for (i = 2; i < P; i++) {
		get_poly_product(coeff, ptable[i-1], ptable[i]);
	}
}

static void dump_coeffs(int *coeff)
{
	int i;

	for (i = 0; i < P; i++)
		printf("%d ", coeff[i]);
	printf("\n");
}

static void dump_poly_power_matrix(void)
{
	int i;

	printf("-------------------------------------------------------\n");
	for (i = 0; i < P; i++) {
		printf("[%3d] ", i);
		dump_coeffs(ptable[i]);
	}
	printf("-------------------------------------------------------\n");
}

static void eval_poly_coeffs(void)
{
	int *temp, *eval, i, j, k = 0;

	temp = (int *)malloc(sizeof(int)*P);
	if (!temp) return;

	eval = (int *)malloc(sizeof(int)*P);
	if (!eval) {
		free(temp);
		return;
	}

	for (i = 0; i < P; i++) {
		eval[i] = coeff[i];
		temp[i] = 0;
	}

	/* compute eval(coeffs) and store it in eval */
	do {
		dump_coeffs(eval);
		for (i = 0; i < P; i++) {
			if (!eval[i]) continue;
			for (j = 0; j < P; j++) {
				temp[j] = (temp[j] + ptable[i][j]*eval[i]) % P;
			}
		}
		for (i = 0; i < P; i++) {
			eval[i] = temp[i];
			temp[i] = 0;
		}
		k++;
	} while (k < P);
}

int main(int argc, char *argv[])
{
	int i;

	scanf("%d", &P);
	if (P <= 1) return -1;

	ptable = (int **)malloc(sizeof(int *)*P);
	if (!ptable) return -1;
	for (i = 0; i < P; i++) {
		ptable[i] = (int *)malloc(sizeof(int)*P);
		if (!ptable[i]) goto _clean_ptable;
	}

	coeff = (int *)malloc(sizeof(int)*P);
	if (!coeff) goto _clean_ptable;

	input_poly_coeffs();
	compute_poly_power_matrix();
	dump_poly_power_matrix();
	eval_poly_coeffs();

	return 0;

_clean_ptable:
	while (--i >= 0)
		free(ptable[i]);
	return -1;
}
