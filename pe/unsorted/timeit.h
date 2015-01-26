#ifndef TIMEIT_H
#define TIMEIT_H

#include <sys/time.h>
#include <time.h>

static struct timeval __tv_start, __tv_peek;
static clock_t __t_cpu_clock_start, __t_cpu_clock_peek;

/* GLIBC: Elapsed time
 *
 * Subtract the ¡®struct timeval¡¯ values X and Y,
 * storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0.
 */
static int __attribute__((unused))
__timeit_timeval_subtract (struct timeval *result,
                           struct timeval *x,
                           struct timeval *y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
     * tv_usec is certainly positive.
     */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

static inline void __attribute__((unused))
timeit_timer_start (void)
{
    struct timezone tz;

    gettimeofday(&__tv_start, &tz);
    __t_cpu_clock_start = clock();
}

static inline void __attribute__((unused))
timeit_timer_peek (void)
{
    struct timezone tz;

    __t_cpu_clock_peek = clock();
    gettimeofday(&__tv_peek, &tz);
}

static inline void __attribute__((unused))
timeit_timer_print (void)
{
    struct timeval t_diff;

    __timeit_timeval_subtract(&t_diff, &__tv_peek, &__tv_start);
    printf("\nELAPSED: (Wallclock: %ld.%ld.%ld s) (CPU: %lf s)\n",
           t_diff.tv_sec, t_diff.tv_usec/1000, (t_diff.tv_usec % 1000),
           (double)(__t_cpu_clock_peek - __t_cpu_clock_start)/CLOCKS_PER_SEC);
    fflush(stdout);
}

static inline void __attribute__((unused))
timeit_timer_peek_and_print (void)
{
    timeit_timer_peek();
    timeit_timer_print();
}

#endif /* TIMEIT_H */
