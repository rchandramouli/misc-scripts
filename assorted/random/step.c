#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

#define MAX(a, b)     (((a) > (b)) ? (a): (b))
#define MIN(a, b)     (((a) < (b)) ? (a): (b))

#define MAX_SAMPLES       1000000
#define MAX_HIST_SAMPLES  10000

static int samples[MAX_SAMPLES];
static int histogram[MAX_HIST_SAMPLES];

static int rfd = -1;
static void rnd_init (void)
{
    if (rfd > 0) {
        return;
    }

    srandom(time(NULL));
    rfd = open("/dev/urandom", O_RDONLY);
}

static long int RND (void)
{
    long int x;

    if (rfd < 0) {
        x = random();
    } else {
        if (read(rfd, &x, sizeof(x)) <= 0) {
            x = random();
        }
    }
    return x;
}

int main (int argc, char *argv[])
{
    int dir = 0, t, s, fbk;
    int min, max;
    double run_avg = 0.0;

    rnd_init();

    min = max = 0;
    samples[0] = 0;
    fbk = 0;

    printf("0 %d 0\n", samples[0]);
    for (s = 1; s < MAX_SAMPLES; s++) {
        /* R1 */ //t = ((RND() + RND() + RND()) % 3UL) - 1;  /* -1, 0, 1 */
        //t = ((((RND() & 0xFF00) >> 8) + ((RND() & 0xFF0000) >> 16)) % 3UL) - 1;
        t = (RND() % 3UL)-1;

        if (dir == -1 && t > 0) {
            t = 0;
        } else if (dir == 1 && t < 0) {
            t = 0;
        }
        samples[s] = samples[s-1] + t;
        dir = t;

        run_avg = ((run_avg * s) + samples[s]) / (s+1);

        min = MIN(min, samples[s]);
        max = MAX(max, samples[s]);

        printf("%d %d %f\n", s, samples[s], run_avg);
    }

    /* Plot the histogram between the maximum and minimum */
    for (s = 0; s < MAX_HIST_SAMPLES; s++) {
        histogram[samples[s] + min]++;
    }
    for (s = min; s <= max; s++) {
        //printf("%d %d\n", s, histogram[s-min]);
    }

    return 0;
}
