#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>

#define N_VERTICES       (21)
#define X_RADIUS         (10)
#define Y_RADIUS         (10)

#define MAX_X         (128)
#define MAX_Y         (128)
#define MAX_Z         (128)

#define MAX_ITER_FWD  (10000)
#define MAX_ITER_REV  (10000)

#define FILL_RANDOM   (1)
#define FILL_CIRCLE   (2)

#define AVG_TWPT_MEAN (1)
#define AVG_THPT_MEAN (2)
#define AVG_RMS_MEAN  (3)

static double x[N_VERTICES], y[N_VERTICES], z[N_VERTICES];
static double ix[N_VERTICES], iy[N_VERTICES], iz[N_VERTICES];
static double rx[N_VERTICES], ry[N_VERTICES], rz[N_VERTICES];

static int fill_type = FILL_RANDOM;
static int avg_type = AVG_TWPT_MEAN;

static void
fill_random_points (int nr)
{
    int i;

    for (i = 0; i < nr; i++) {
        x[i] = rand() % MAX_X;
        y[i] = rand() % MAX_Y;
        z[i] = rand() % MAX_Z;

        ix[i] = rx[i] = x[i];
        iy[i] = ry[i] = y[i];
        iz[i] = rz[i] = z[i];
    }
}

static void
fill_circular_points (int nr, int max_n)
{
    int i;

    for (i = 0; i < nr; i++) {
        x[i] = (MAX_X + MAX_X * cos(((double)i)*2.0*M_PI/(double)max_n)) / 2.0;
        y[i] = (MAX_Y + MAX_Y * sin(((double)i)*2.0*M_PI/(double)max_n)) / 2.0;

        ix[i] = rx[i] = x[i];
        iy[i] = ry[i] = y[i];
    }
}

static void
mean_normalize (double *arr, int nr)
{
    int i, renorm = 1;
    double avg, p = 10.0, v;

    for (avg = 0.0, i = 0; i < nr; i++)
        avg += arr[i];
    avg /= (double)nr;

    for (i = 0; i < nr; i++) {
        arr[i] -= avg;
        if (arr[i] > 1.0 || arr[i] < -1.0)
            renorm = 0;
    }

    if (renorm) {
        for (i = 0; i < nr; i++) {
            v = arr[i] * p;
            while ((v < 1.0 && v > -1.0)) {
                p *= 10.0;
                v = arr[i] * p;
            }
        }
        for (i = 0; i < nr; i++) {
            arr[i] *= p;
        }
    }
}

static void
compute_twpt_average (double *arr, int nr, int iter)
{
    int i, j;
    double u;

    for (i = 0; i < iter; i++) {
        u = arr[0];
        for (j = 0; j < nr-1; j++) {
            arr[j] = (arr[j] + arr[j+1]) / 2.0;
        }
        arr[nr-1] = (arr[nr-1] + u) / 2.0;

        mean_normalize(arr, nr);
    }
}

static void
compute_thpt_average (double *arr, int nr, int iter)
{
    int i, j;
    double u, v;

    for (i = 0; i < iter; i++) {
        u = arr[0];
        v = arr[1];
        for (j = 0; j < nr-2; j++) {
            arr[j] = (arr[j] + arr[j+1] + arr[j+2]) / 3.0;
        }
        arr[nr-2] = (arr[nr-2] + arr[nr-1] + u) / 3.0;
        arr[nr-1] = (arr[nr-1] + u + v) / 3.0;

        mean_normalize(arr, nr);
    }
}

static void
compute_rms_average (double *arr, int nr, int iter)
{
    int i, j;
    double u;

    for (i = 0; i < iter; i++) {
        u = arr[0];
        for (j = 0; j < nr-1; j++) {
            arr[j] = sqrt((arr[j]*arr[j] + arr[j+1]*arr[j+1])/2.0);
        }
    }

    /* Apply mean-normalization at the end */
    mean_normalize(arr, nr);
}

static void
compute_inv_twpt_average (double *arr, int nr, int iter, double scale)
{
    int i, j;
    double u, v;

    for (i = 0; i < iter; i++) {
        v = arr[0];

        u = 0.0;
        if (!(nr & 0x1)) {
            /* nr * u1 = (nr-1)*x_1 - (nr-2)*x_2 + ... + x_nr-1 */
            for (j = 0; j < nr-1; j++) {
                u = (nr-j-1)*arr[j] - u;
            }
            u /= (double)nr;
        } else {
            for (j = nr-1; j >= 0; j--) {
                u = arr[j] - u;
            }
        }
        arr[0] = u;
        for (j = 1; j < nr; j++) {
            u = arr[j];
            arr[j] = 2.0*v - arr[j-1];
            v = u;
        }
        /* Normalize */
        for (j = 0; j < nr; j++) {
            arr[j] /= (double)nr / scale;
        }
    }
    mean_normalize(arr, nr);
}

int main (int argc, char *argv[])
{
    int i;

    srand(time(NULL));

    if (fill_type == FILL_RANDOM) {
        fill_random_points(N_VERTICES);

    } else if (fill_type == FILL_CIRCLE) {
        fill_circular_points(N_VERTICES, N_VERTICES);
    }

    if (avg_type == AVG_TWPT_MEAN) {
        compute_twpt_average(ix, N_VERTICES, MAX_ITER_FWD);
        compute_twpt_average(iy, N_VERTICES, MAX_ITER_FWD);
        compute_twpt_average(iz, N_VERTICES, MAX_ITER_FWD);

    } else if (avg_type == AVG_THPT_MEAN) {
        compute_thpt_average(ix, N_VERTICES, MAX_ITER_FWD);
        compute_thpt_average(iy, N_VERTICES, MAX_ITER_FWD);
        compute_thpt_average(iz, N_VERTICES, MAX_ITER_FWD);

    } else if (avg_type == AVG_RMS_MEAN) {
        compute_rms_average(ix, N_VERTICES, MAX_ITER_FWD);
        compute_rms_average(iy, N_VERTICES, MAX_ITER_FWD);
        compute_rms_average(iz, N_VERTICES, MAX_ITER_FWD);
    }

    if (fill_type == FILL_RANDOM) {
        compute_inv_twpt_average(rx, N_VERTICES, MAX_ITER_REV, 1.5645);
        compute_inv_twpt_average(ry, N_VERTICES, MAX_ITER_REV, 1.5645);
        compute_inv_twpt_average(rz, N_VERTICES, MAX_ITER_REV, 1.5645);

    } else if (fill_type == FILL_CIRCLE) {
        compute_inv_twpt_average(rx, N_VERTICES, MAX_ITER_REV, 1.6020);
        compute_inv_twpt_average(ry, N_VERTICES, MAX_ITER_REV, 1.6020);
        compute_inv_twpt_average(rz, N_VERTICES, MAX_ITER_REV, 1.6020);
    }

    for (i = 0; i < N_VERTICES; i++) {
        printf("A: %lf %lf %lf\n", x[i], y[i], z[i]);
        printf("B: %lf %lf %lf\n", ix[i], iy[i], iz[i]);
        printf("C: %lf %lf %lf\n", rx[i], ry[i], rz[i]);
    }
    printf("A: %lf %lf %lf\n", x[0], y[0], z[0]);
    printf("B: %lf %lf %lf\n", ix[0], iy[0], iz[0]);
    printf("C: %lf %lf %lf\n", rx[0], ry[0], rz[0]);

    for (i = 0; i < N_VERTICES; i+=2)
        printf("D: %lf %lf %lf\n", rx[i], ry[i], rz[i]);
    for (i = 1; i < N_VERTICES; i+=2)
        printf("D: %lf %lf %lf\n", rx[i], ry[i], rz[i]);
    printf("D: %lf %lf %lf\n", rx[0], ry[0], rz[0]);

    /* Projection points of final curve */
    /* XY */
    for (i = 0; i < N_VERTICES; i++)
        printf("B-XY: %lf %lf 0.0\n", ix[i], iy[i]);
    printf("B-XY: %lf %lf 0.0\n", ix[0], iy[0]);
    /* YZ */
    for (i = 0; i < N_VERTICES; i++)
        printf("B-YZ: 0.0 %lf %lf\n", iy[i], iz[i]);
    printf("B-YZ: 0.0 %lf %lf\n", iy[0], iz[0]);
    /* XZ */
    for (i = 0; i < N_VERTICES; i++)
        printf("B-XZ: %lf 0.0 %lf\n", ix[i], iz[i]);
    printf("B-XZ: %lf 0.0 %lf\n", ix[0], iz[0]);

    return 0;
}
