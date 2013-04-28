#include <stdio.h>

static double a1, b1, c1, a2, b2, c2;
static double a3, b3, c3, a4, b4, c4;

#define TR_X(x, y)  ((a1*(x)+b1*(y)+c1)/(a2*(x)+b2*(y)+c2))
#define TR_Y(x, y)  ((a3*(x)+b3*(y)+c3)/(a4*(x)+b4*(y)+c4))

static void dump_points (double xs, double xe, double xinc,
			 double ys, double ye, double yinc)
{
    double x, y, xnew, ynew;

    for (x = xs; x <= xe; x += xinc) {
	for (y = ys; y <= ye; y += yinc) {
	    xnew = TR_X(x, y);
	    ynew = TR_Y(x, y);
	    printf("%lf %lf\n", xnew, ynew);
	}
    }
}

static void dump_points_symmetric (double s, double e, double inc)
{
    dump_points(s, e, inc, s, e, inc);
}

static void dump_lines (double sslope, double eslope, double inc)
{
    double x, y, s;

    for (s = sslope; s <= eslope; s += inc) {
	for (x = -100; x <= 100; x++) {
	    y = s * x;
	    printf("%lf %lf\n", TR_X(x, y), TR_Y(x, y));
	}
	printf("\n");
    }
}

int main (int argc, char *argv[])
{
    scanf("%lf %lf %lf %lf %lf %lf "
	  "%lf %lf %lf %lf %lf %lf",
	  &a1, &b1, &c1, &a2, &b2, &c2,
	  &a3, &b3, &c3, &a4, &b4, &c4);

    /* Map x,y */
    //dump_points_symmetric(0.0, 3.0, 0.01);
    //dump_points_symmetric(10.0, 1000.0, 50.0);
    //dump_points_symmetric(1000.0, 100000.0, 5000.0);

    dump_lines(0.1, 3.0, 0.1);
    dump_lines(10.0, 1000.0, 50.0);
    dump_lines(1000.0, 100000.0, 5000.0);

    return 0;
}
