#include <stdio.h>
#include <stdarg.h>

#define DUMPER_PAGE_SZ           (4096 * 1024)

#define DUMPER_OVERWRITE         0x01
#define DUMPER_CLEAR_ON_DUMP     0x02

struct dumper {
	char buffer[DUMPER_PAGE_SZ];
	int head;
	int dflags;

	int (*bprintf)(char *buf, const char *fmt, va_list args);
};

static inline void dumper_reset(struct dumper *d)
{
	d->head = 0;
	d->buffer[0] = 0;
}

static inline void dumper_set_flags(struct dumper *d, int flg)
{
	d->dflags |= flg;
}

static inline void dumper_clear_flags(struct dumper *d, int flg)
{
	d->dflags &= ~flg;
}

static inline void dumper_init(struct dumper *d)
{
	dumper_reset(d);	
	d->dflags = 0;
	d->bprintf = vsprintf;
}

static inline void dump_to_stdout(struct dumper *d)
{
	fprintf(stdout, d->buffer);
	if (d->dflags & DUMPER_CLEAR_ON_DUMP)
		dumper_reset(d);
}

static int print_to_dumper(struct dumper *d, const char *fmt, ...)
{
	va_list args;
	int ret, len;

	if (d->head >= DUMPER_PAGE_SZ) {
		if (!(d->dflags & DUMPER_OVERWRITE))
			return 0;
		dumper_reset(d);
	}

	va_start(args, fmt);
	ret = d->bprintf(d->buffer + d->head, fmt, args);
	va_end(args);

	if (ret < 0) {
		fprintf(stderr, "%s: unknown print error (%d)!\n",
			__func__, ret);
		return ret;
	}

	len = d->head;
	d->head += ret;
	if (d->head > DUMPER_PAGE_SZ)
		d->head = DUMPER_PAGE_SZ-1;
	d->buffer[d->head] = 0;

	return d->head-len+1;
}

/*---------------------------------------------------------------------------*/

static int seqdiv[32768], seqpdt[32768], k;
static struct dumper seqdump;

static int seq_formulae(int n)
{
	int i;

	if (n <= 0) return -1;

	for (i = 0; i < n; i++) {
		if ((i+1)%25 == 0)
			printf("\n");
		printf("%2d ", (i/3)+((i+1)/6));
	}
	return 0;
}

static void dump_seq(void)
{
	int i;

	for (i = 0; i < k; i++) {
		printf("%2d[%3d] ", seqpdt[i], seqdiv[i]);
		if (((i+1) % 20) == 0)
			printf("\n");
	}
	printf("\n");
}

/* f(n) = floor(n/3) + floor((n+1)/3 */
static int gen_seq_1(int n)
{
	int i, j, u, v;

	seqdiv[0] = 3; seqpdt[0] = 1;
	k = 1;
	for (i = 0; i < n; i++) {
		for (j = v = 0; j < k; j++) {
			v += seqpdt[j]*(i/seqdiv[j]);
		}
		u = (i/3)+((i+1)/6);
		if (u > v) {
			seqdiv[k] = i;
			seqpdt[k] = u-v;
			k++;
		} else if (u < v) {
			seqdiv[k] = -i;
			seqpdt[k] = v-u;
			k++;
		}
	}

	return 0;
}

/* Dump LaTeX Math environment code. */
static void dump_seq_2(void)
{
	int i, s;

	printf("n = 1 + (");
	for (i = 0; i < k; i++) {
		if (seqdiv[i] < 0) continue;

		if (seqpdt[i] > 1)
			printf("%d", seqpdt[i]);
		printf("\\lfloor{\\frac{n}{%d}}\\rfloor + ", seqdiv[i]);
	}
	printf("...) - (");
	for (i = 0; i < k; i++) {
		if (seqdiv[i] > 0) continue;

		if (seqpdt[i] > 1)
			printf("%d", seqpdt[i]);
		printf("\\lfloor{\\frac{n}{%d}}\\rfloor + ", -seqdiv[i]);
	}
	printf("...)\n");
#if 0
	for (i = 0; i < k; i++) {
		s = (seqdiv[i] < 0) ? -1: 1;
		printf((s < 0) ? " - ": " + ");
		if (seqpdt[i] > 1)
			printf("%d", seqpdt[i]);
		printf("\\lfloor{\\frac{n}{%d}}\\rfloor", s*seqdiv[i]);
	}
	printf("\n");
#endif
}

/* f(n) = n */
static int gen_seq_2(int n)
{
	int i, j, u, v;

	seqdiv[0] = 2; seqpdt[0] = 1;
	k = 1;
	for (i = 2; i <= n; i++) {
		for (j = 0, v = 1; j < k; j++) {
			v += seqpdt[j]*(i/seqdiv[j]);
		}
		u = i;
		if (u > v) {
			seqdiv[k] = i;
			seqpdt[k] = u-v;
			k++;
		} else if (u < v) {
			seqdiv[k] = -i;
			seqpdt[k] = v-u;
			k++;
		}
	}

	dump_seq_2();

	return 0;
}

static void dump_seq_3(void)
{
	int i;

	printf("\\binom{n+1}{2} = 1 + (");
	for (i = 0; i < k; i++) {
		if (seqpdt[i] < 0) continue;
		if (seqpdt[i] > 1)
			printf("%d", seqpdt[i]);
		printf("\\lfloor{\\frac{n}{%d}}\\rfloor + ", seqdiv[i]);
	}
	printf("...) - (");
	for (i = 0; i < k; i++) {
		if (seqpdt[i] > 0) continue;
		if (seqpdt[i] < -1)
			printf("%d", -seqpdt[i]);
		printf("\\lfloor{\\frac{n}{%d}}\\rfloor + ", seqdiv[i]);
	}
	printf("...)\n");
}

/* f(n) = n*(n+1)/2 */
static int gen_seq_3(int n)
{
	int i, j, u, v;

	seqdiv[0] = 1; seqpdt[0] = 0;
	k = 0;

	for (i = 1; i < n; i++) {
		u = i*(i+1)/2;
		v = 1;

		for (j = 0; j < k; j++) {
			v += seqpdt[j]*(i/seqdiv[j]);
		}
		if (u == v) continue;

		seqdiv[k] = i;
		seqpdt[k] = u-v;
		k++;
	}
	dump_seq_3();

	return 0;
}

/* f(n) = floor(n/k),  2 <= k < inf */
/* A002541 */
static int gen_seq_4(int n)
{
	int i, v;

	for (i = 2; i <= n; i++) {
		for (k = 2, v = 0; k <= i; k++)
			v += i/k;
		printf("%3d, ", v);
	}
	printf("\n");

	return 0;
}

/* f(n) = n*floor((f(n-1)+f(n-2))/n) + 1, f(0) = 1, f(1) = 1 */
/* g(n) = g(n-1) + g(n-2) + 1, g(0) = 1, g(1) = 1 */
/* D = g(n)-f(n) */
static int gen_seq_5(int n)
{
	int i, v0, v1, v2, u0, u1, u2;

	for (v0 = v1 = u0 = u1 = i = 1; i <= n; i++) {
		printf("%3d, ", u0-v0);
		v2 = ((v0 + v1) / i);
		v2 = i*v2 + 1;
		v0 = v1;
		v1 = v2;
		u2 = u0 + u1 + 1;
		u0 = u1;
		u1 = u2;
	}
	printf("\n");

	return 0;
}

static void dump_seq_6(int tex)
{
	int i;
	struct dumper *d = &seqdump;
	char *fmt1, *fmt2, *fmt3, *fmt4;

	if (tex) {
		fmt1 = "n^2 = 1 + (";
		fmt2 = "%d";
		fmt3 = "\\lfloor{\\frac{n}{%d}}\\rfloor + ";
		fmt4 = "...) = (";
		fmt5 = "...)\n";
	} else {
		fmt1 = "n^2: \t+(1, ";
		fmt2 = "%3d*";
		fmt3 = "%d, ";
		fmt4 = "...)\n\t-(";
		fmt5 = "...)\n";
	}

	print_to_dumper(d, fmt1);
	for (i = 0; i < k; i++) {
		if (seqpdt[i] < 0) continue;
		if (seqpdt[i] > 1)
			print_to_dumper(d, fmt2, seqpdt[i]);
		print_to_dumper(d, fmt3, seqdiv[i]);
	}
	print_to_dumper(d, fmt4);
	for (i = 0; i < k; i++) {
		if (seqpdt[i] > 0) continue;
		if (seqpdt[i] < -1)
			print_to_dumper(d, fmt2, -seqpdt[i]);
		print_to_dumper(d, fmt3, seqdiv[i]);
	}
	print_to_dumper(d, fmt5);
}

/* n^2 = floor(x)+... */
static int gen_seq_6(int n)
{
	int i, j, u, v;

	seqdiv[0] = 1; seqpdt[0] = 0;
	k = 0;

	for (i = 1; i < n; i++) {
		u = i*i;
		v = 1;

		for (j = 0; j < k; j++) {
			v += seqpdt[j]*(i/seqdiv[j]);
		}
		if (u == v) continue;

		seqdiv[k] = i;
		seqpdt[k] = u-v;
		k++;
	}
	dump_seq_6();

	return 0;
}

int main(int argc, char *argv[])
{
	int n;

	scanf("%d", &n);
	if (n <= 0) return -1;

	dumper_init(&seqdump);
	dumper_set_flags(&seqdump, DUMPER_CLEAR_ON_DUMP);

	//gen_seq_1(n);
	//gen_seq_2(n);
	//gen_seq_3(n);
	//gen_seq_4(n);
	//gen_seq_5(n);
	gen_seq_6(n);

	dump_to_stdout(&seqdump);

	return 0;
}
