/* Solve for:
 * x1 + 2x2 + 5x3 + 10x4 + 20x5 + 50x6 + 100x7 + 200x8 = 200
 */
#include <stdio.h>

/* Do it in a trivial way */
#define EXVAL()     (x1 + 2*x2 + 5*x3 + 10*x4 + 20*x5 + 50*x6 + 100*x7 + 200*x8)

#define FOR(x, s, e)   for(x = s; x <= e; x++)
#define CHECK_VAL()							\
	if (EXVAL() > 200) break;					\
	if (EXVAL() == 200) {						\
		printf("(%3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d)\n",	\
		       x1, x2, x3, x4, x5, x6, x7, x8);			\
		count++;						\
		continue;						\
	}

int main (int argc, char *argv[])
{
	int count = 0;
	int x1 = 0, x2 = 0, x3 = 0, x4 = 0, x5 = 0, x6 = 0, x7 = 0, x8 = 0;

	FOR(x1, 0, 200) {
		CHECK_VAL();
		FOR(x2, 0, 100) {
			CHECK_VAL();
			FOR(x3, 0, 40) {
				CHECK_VAL();
				FOR(x4, 0, 20) {
					CHECK_VAL();
					FOR(x5, 0, 10) {
						CHECK_VAL();
						FOR(x6, 0, 4) {
							CHECK_VAL();
							FOR(x7, 0, 2) {
								CHECK_VAL();
								FOR(x8, 0, 1) {
									CHECK_VAL();
								}
								x8 = 0;
							}
							x7 = 0;
						}
						x6 = 0;
					}
					x5 = 0;
				}
				x4 = 0;
			}
			x3 = 0;
		}
		x2 = 0;
	}
	printf("%d\n", count);
	return 0;
}
