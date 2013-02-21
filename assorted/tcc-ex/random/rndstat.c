#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "random.h"

#define MAX_BUCKETS     256

struct random_stats {
	int nr_samples;
	int freq[MAX_BUCKETS];

	unsigned char *samples;
};

static struct random_stats rstat;

static void fill_xored_random_samples(struct random_stats *rs)
{
	int i;

	rs->samples[0] = RND() & 0xff;
	for (i = 1; i < rs->nr_samples; i++)
		rs->samples[i] = (RND() & 0xff) ^ rs->samples[i-1];
}

static void fill_random_samples(struct random_stats *rs)
{
	int i;

	for (i = 1; i < rs->nr_samples; i++)
		rs->samples[i] = RND() & 0xff;
}

static void dump_average_deviations(struct random_stats *rs, int nr)
{
	int i;
	unsigned long int avg = 0;

	if (!nr) return;

	for (i = 0; i < nr; i++)
		avg += rs->freq[i];
	avg /= nr;

	for (i = 0; i < nr; i++)
		printf("%d %ld\n", i, rs->freq[i]-(long int)avg);
}

static void compute_frequency_curve(struct random_stats *rs)
{
	int i;

	for (i = 0; i < MAX_BUCKETS; i++)
		rs->freq[i] = 0;

	for (i = 0; i < rs->nr_samples; i++)
		rs->freq[rs->samples[i]]++;

	dump_average_deviations(rs, MAX_BUCKETS);
}

static void compute_bit_frequencies(struct random_stats *rs)
{
	int i, j;
	unsigned char ch;

	for (i = 0; i < 8; i++)
		rs->freq[i] = 0;

	for (i = 0; i < rs->nr_samples; i++) {
		j = 0;
		ch = rs->samples[i];
		while (ch) {
			if (ch & 1) rs->freq[j]++;
			ch >>= 1;
			j++;
		}
	}

	dump_average_deviations(rs, 8);
}

int main(int argc, char *argv[])
{
	int rtype;

	scanf("%d %d", &rtype, &rstat.nr_samples);
	if (rstat.nr_samples <= 0) return -1;

	rstat.samples = (unsigned char *)malloc(sizeof(unsigned char) * rstat.nr_samples);
	if (!rstat.samples) return -1;

	init_random((rtype) ? RND_ENTROPY: RND_PSEUDO);

	fill_xored_random_samples(&rstat);
	compute_frequency_curve(&rstat);
	//compute_bit_frequencies(&rstat);

	fini_random();
	free(rstat.samples);

	return 0;
}
