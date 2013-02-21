#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef unsigned int u32;
typedef unsigned long long int u64;

#define MAX_LINE_PACKETS        142045454

/* random number */
static int rfd = -1;

void initrng (void)
{
	rfd = open("/dev/urandom", O_RDONLY);
	if (rfd < 0) {
		perror("open");
		exit(-1);
	}
}

u32 rng_get_sample (void)
{
	u32 random;

	if (rfd < 0) return 0;
	read(rfd, &random, sizeof(random));

	return random;
}

void finirng (void)
{
	close(rfd);
}

#define RANDOMIZE_STAGE1
#define RANDOMIZE_STAGE2

#define eqRND(m, s)   (rng_get_sample() & (((m)<<(s))-1))

int main (int argc, char *argv[])
{
	int i;
	u32 sample_rate, stage1, stage2, runs;
	u32 ctr_stage1, ctr_stage2;
	u64 pkt_counter, stage1_sample, stage2_sample;

	scanf("%d %u %u", &runs, &sample_rate, &stage1);
	stage2 = sample_rate / stage1;

	printf("%u %u %u\n", sample_rate, stage1, stage2);
	initrng();

	for (i = 0; i < runs; i++) {
		stage1_sample = stage2_sample = 0;
		ctr_stage1 &= (stage1 - 1);
		ctr_stage2 &= (stage2 - 1);

		for (pkt_counter = 0, ctr_stage1 = stage1; pkt_counter <= MAX_LINE_PACKETS; pkt_counter++) {
			/* stage1 */
#ifdef RANDOMIZE_STAGE1
			if (!ctr_stage1) {
				stage1_sample++;
				if (!(ctr_stage1 = eqRND(stage1, 1))) {
					ctr_stage1 = eqRND(stage1, 1);
				}
			} else {
				ctr_stage1--;
				continue;
			}
#else
			if (!ctr_stage1) {
				stage1_sample++;
				ctr_stage1 = stage1;
			} else {
				ctr_stage1--;
				continue;
			}
#endif
		/* stage2 */
#ifdef RANDOMIZE_STAGE2
			if (!ctr_stage2) {
				stage2_sample++;
				if (!(ctr_stage2 = eqRND(stage2, 1))) {
					ctr_stage2 = eqRND(stage2, 1);
				}
			} else {
				ctr_stage2--;
				continue;
			}
#else
			if (!ctr_stage2) {
				stage2_sample++;
				ctr_stage2 = stage2;
			} else {
				ctr_stage2--;
				continue;
			}
#endif
		}

		printf("%4d Ratio = %Lu / %5Lu (%.3Lf), stage1 = %Lu, stage2 = %Lu\n",
		       i+1, (u64)MAX_LINE_PACKETS, stage2_sample,
		       ((long double)MAX_LINE_PACKETS/(long double)stage2_sample),
		       stage1_sample, stage2_sample);
	}

	return 0;
}
