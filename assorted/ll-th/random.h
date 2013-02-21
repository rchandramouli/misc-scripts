#ifndef RANDOM_H
#define RANDOM_H

/*----------------------------------------------------------------------------
 * Random buffer routines 
 *---------------------------------------------------------------------------*/
static int rfd;

static void init_random(void)
{
	if (rfd)
		return;

	rfd = open("/dev/urandom", O_RDONLY);
	if (rfd < 0)
		srandom(time(NULL));
}

static void fini_random(void)
{
	if (rfd > 0)
		close(rfd);
}

static void get_random_bytes(unsigned char *buf, int len)
{
	int i;
	unsigned int r;

	if (rfd > 0) {
		read(rfd, buf, len);
		return;
	}

	for (i = 0; i+4 < len; i += 4) {
		r = random();
		*(int *)(&buf[i]) = r;
	}
	r = random();

	switch (len % 4) {
	case 3:
		buf[i++] = (unsigned char)(r & 0xff);
		r >>= 8;
	case 2:
		buf[i++] = (unsigned char)(r & 0xff);
		r >>= 8;
	case 1:
		buf[i++] = (unsigned char)(r & 0xff);
	}
}

#endif
