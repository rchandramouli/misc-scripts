#ifndef RANDOM_H
#define RANDOM_H

enum {
	RND_PSEUDO,
	RND_ENTROPY,
};

/* generate random from /dev/urandom */
static int rfd = -1;
static long int (*get_random)(void);

#define RND()    ((*get_random)())

static long int get_urandom(void)
{
  long int bytes;

  return (read(rfd, &bytes, sizeof(bytes)) < 0) ? random(): bytes;
}

static inline void init_random(int type)
{
	if (type == RND_ENTROPY) {
		rfd = open("/dev/urandom", O_RDONLY);
		if (rfd < 0)
			type = RND_PSEUDO;
	}
	if (type == RND_PSEUDO) {
		srandom(time(NULL));
		get_random = random;
	} else {
		get_random = get_urandom;
	}
}

static inline void fini_random(void)
{
  if (rfd > 0)
    close(rfd);
}

#endif // RANDOM_H
