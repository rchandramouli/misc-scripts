#include <stdio.h>

/* The matrix:
 * Appropriately numbered as functions: 11/12/13, 21/22/23, 31/32/33
 *
 * XOR(a+1, -a+1)   XOR(a+1, -a)   XOR(a+1, -a-1)
 * XOR(a, -a+1)     XOR(a, -a)     XOR(a, -a-1)
 * XOR(a-1, -a+1)   XOR(a-1, -a)   XOR(a-1, -a-1)
 */

static void compute_vals(int a)
{
	printf("\t\t\ta = %3d,\n"
	       "(a+1)^(-a+1) = %3d, (a+1)^(-a) = %3d, (a+1)^(-a-1) = %3d\n"
	       "    a^(-a+1) = %3d,     a^(-a) = %3d,     a^(-a-1) = %3d\n"
	       "(a-1)^(-a+1) = %3d, (a-1)^(-a) = %3d, (a-1)^(-a-1) = %3d\n",
	       a,
	       (a+1)^(-a+1), (a+1)^(-a), (a+1)^(-a-1),
	       a^(-a+1), a^(-a), a^(-a-1),
	       (a-1)^(-a+1), (a-1)^(-a), (a-1)^(-a-1));
}

static void dump_cycle(int *cycle, int len)
{
	int i;

	for (i = 0; i < len; i++)
		printf("%2d, ", cycle[i]);
	printf("\n");
}

#define strify(x)    #x
#define stringify(x) strify(x)

#define F11(x)  (((x)+1)^(-(x)+1))
#define F12(x)  (((x)+1)^(-(x)))
#define F13(x)  (((x)+1)^(-(x)-1))
#define F21(x)  ((x)^(-(x)+1))
#define F22(x)  ((x)^(-(x)))
#define F23(x)  ((x)^(-(x)-1))
#define F31(x)  (((x)-1)^(-(x)+1))
#define F32(x)  (((x)-1)^(-(x)))
#define F33(x)  (((x)-1)^(-(x)-1))

#define DEFINE_COMPUTE_CYCLE(n)					\
	static void compute_cycle_ ## n(int a) {		\
		int v = a, cycle[4096], len = 1, found = 0, i;	\
								\
		printf("**** PROC: %s ----> %s\n",		\
		       __func__, stringify(F ## n(t)));		\
		cycle[0] = a;					\
		while (!found && len < 4096) {			\
			v = F ## n(v);				\
			for (i = 0; !found && i < len; i++) {	\
				if (cycle[i] == v)		\
					found = 1;		\
			}					\
			cycle[len] = v;				\
			len++;					\
		}						\
		if (!found) {					\
			printf("a = %d, [no loops]\n", a);	\
		} else {					\
			printf("a = %d, [loop at %d(%d)]\n",	\
			       a, i, cycle[i]);			\
		}						\
		dump_cycle(cycle, len);				\
		printf("\n");					\
	}

DEFINE_COMPUTE_CYCLE(11)
DEFINE_COMPUTE_CYCLE(12)
DEFINE_COMPUTE_CYCLE(13)
DEFINE_COMPUTE_CYCLE(21)
DEFINE_COMPUTE_CYCLE(22)
DEFINE_COMPUTE_CYCLE(23)
DEFINE_COMPUTE_CYCLE(31)
DEFINE_COMPUTE_CYCLE(32)
DEFINE_COMPUTE_CYCLE(33)

int main(int argc, char *argv[])
{
	int n;

	scanf("%d", &n);

	compute_cycle_11(n);
	compute_cycle_12(n);
	compute_cycle_13(n);
	compute_cycle_21(n);
	compute_cycle_22(n);
	compute_cycle_23(n);
	compute_cycle_31(n);
	compute_cycle_32(n);
	compute_cycle_33(n);

	return 0;
}
