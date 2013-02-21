#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

#include "libtcc.h"
#include "IL/il.h"

#include "random.h"

struct codebuf {
	TCCState *tcc;
	void *mem;

	ILuint *handle;
	int nr_images;

	int (*rgb)(int x, int y);
};

static char dyncode[] = 
	"int get_rgb(int x, int y) {\n"
	"int eval;\n"
	"%s\n"
	"return eval;\n"
	"}";

static void release_cb(struct codebuf *cb)
{
	free(cb->mem);
	cb->mem = NULL;
}

static void *get_sym_ref(struct codebuf *cb, char *sym)
{
	return tcc_get_symbol(cb->tcc, sym);
}

static int compile_snippet(struct codebuf *cb, char *code)
{
	int sz;

	if (tcc_compile_string(cb->tcc, code) == -1)
		return -1;

	sz = tcc_relocate(cb->tcc, NULL);
	if (sz == -1)
		return -1;

	cb->mem = malloc(sz);
	if (!cb->mem) return -1;

	tcc_relocate(cb->tcc, cb->mem);

	return 0;
}

static void tcc_math_lib_init(struct codebuf *cb)
{
	tcc_add_library(cb->tcc, "m");
}

static int il_lib_init(struct codebuf *cb, int nimgs)
{
	ilInit();
	cb->handle = malloc(sizeof(ILuint) * nimgs);
	if (!cb->handle)
		return -1;

	cb->nr_images = nimgs;
	ilGenImages(nimgs, cb->handle);

	ilEnable(IL_FILE_OVERWRITE);

	return nimgs;
}

static int tcc_lib_init(struct codebuf *cb, char *libpath)
{
	cb->tcc = tcc_new();
	if (!cb->tcc) {
		fprintf(stderr, "could not create tcc state\n");
		return -1;
	}

	if (libpath)
		tcc_set_lib_path(cb->tcc, libpath);

	tcc_set_output_type(cb->tcc, TCC_OUTPUT_MEMORY);
	tcc_math_lib_init(cb);

	return 0;
}

static void paint_image(struct codebuf *cb)
{
	ILubyte *pixels;
	int width, height, x, y, pos;

	width = height = 256;

	ilTexImage(width, height, 0, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
	pixels = malloc(sizeof(ILubyte)*3*width*height);

	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			pos = 3*(x + y*width);
			/*
			  pixels[pos] = (x*y) & 0xff;
			  pixels[pos+1] = (x*x+y) & 0xff;
			  pixels[pos+2] = (x+y*y) & 0xff;
			*/
			pixels[pos] = pixels[pos+1] = pixels[pos+2] = (*(cb->rgb))(x, y) & 0xff;
		}
	}

	ilSetPixels(0, 0, 0, width, height, 0, IL_RGB, IL_UNSIGNED_BYTE, pixels);
	ilSaveImage("quad.png");
}

static int thue[1024];
static int mod_add[4096];

static void precompute_thue(int sz)
{
  int i;

  thue[0] = 0;
  for (i = 1; i < sz; i++) {
    thue[i] = (i & 1) ? (thue[i-1] ^ 1): thue[i>>1];
  }
}

static void compute_xor_thue(int sz)
{
  int i;

  thue[0] = 0;
  for (i = 1; i < sz; i++) {
    thue[i] ^= thue[i-1];
  }
}

static void precompute_mod_add(int sz)
{
  int i;

  mod_add[0] = 1;
  for (i = 1; i < sz; i++)
    mod_add[i] = 0;
}

static void compute_mod_add(int sz, int p)
{
  int i;

  mod_add[0] = 1;
  for (i = 1; i < sz; i++)
    mod_add[i] = (mod_add[i-1] + mod_add[i]) % p;
}

static void paint_thue_morse(struct codebuf *cb)
{
	ILubyte *pixels;
	int width, height, x, y, pos;

	width = height = 4096;

	//precompute_thue(width);
	precompute_mod_add(width);

	ilTexImage(width, height, 0, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
	pixels = malloc(sizeof(ILubyte)*3*width*height);

	for (y = 0; y < height; y++) {
	  for (x = 0; x < width; x++) {
	    pos = 3*(x + y*width);
	    //pixels[pos] = pixels[pos+1] = pixels[pos+2] = thue[x % 1024] ? 0xff: 0x00;
	    pixels[pos] = pixels[pos+1] = pixels[pos+2] = (0x00ff * mod_add[x % width])/17;
	  }
	  //compute_xor_thue(width);
	  compute_mod_add(width, 17);
	}

	ilSetPixels(0, 0, 0, width, height, 0, IL_RGB, IL_UNSIGNED_BYTE, pixels);
	ilSaveImage("thue.png");
}

static inline int get_ones(unsigned long int n)
{
  int c = 0;

  while (n) { c++; n &= n-1; }
  return c;
}

static unsigned int get_reduced_byte(unsigned long int n)
{
  unsigned int w, x, y, z;
  unsigned int S = n;
  int c = 0;

  while (S > 0x100) {
    c++;
    if (c > 40) {
	    printf("0x%8x does not reduce in 20 iterations, S = 0x%8x\n", (unsigned int)n, S);
      break;
    }

    w = S & 0xff;
    x = (S >> 8) & 0xff;
    y = (S >> 16) & 0xff;
    z = (S >> 24) & 0xff;
    S = w*y+x*z;
  }

  return S;
}

static inline unsigned char lrot8(unsigned char c, int n)
{
	return ((c << n) | ((((1 << (8-n))-1) & c) >> n));
}

static inline unsigned char rrot8(unsigned char c, int n)
{
	return ((c >> n) | ((((1 << n)-1) & c) << (8-n)));
}

static void compute_rgb_func1(unsigned long int clr, ILubyte *px)
{
	ILubyte r, g, b, x;
	unsigned int c;

#define __char_at(n, i)   ((unsigned char *)(&n))[i]
	c = clr & 0xffffffff;
	x = __char_at(c, 0);
	r = __char_at(c, 1);
	g = __char_at(c, 2);
	b = __char_at(c, 3);
#undef __char_at
	px[2] = lrot8(r, 4)^lrot8(g, 4)^lrot8(b, 4)^lrot8(x, 4);
	px[0] = rrot8(x^r^px[2], 4);
	px[1] = rrot8(r^g^px[0], 4);
	px[2] = rrot8(g^b^px[1], 4);
}

static void compute_rgb_func2(unsigned long int clr, ILubyte *px)
{
	ILubyte r, g, b, x;
	unsigned int c;

#define __char_at(n, i)   ((unsigned char *)(&n))[i]
#define __doxor(x, r, g, b) (((((r)^(g))*((r)^(b)))^(x)) & 0xff)

	c = clr & 0xffffffff;
	x = __char_at(c, 0);
	r = __char_at(c, 1);
	g = __char_at(c, 2);
	b = __char_at(c, 3);

	px[0] = __doxor(x, r, g, b);
	px[1] = __doxor(x, g, b, r);
	px[2] = __doxor(x, b, r, g);

	px[0] = __doxor(px[0], x, px[1], px[2]);
	px[1] = __doxor(px[1], x, px[2], px[0]);
	px[2] = __doxor(px[2], x, px[0], px[1]);
#undef __char_at
#undef __doxor
}

static void compute_rgb_bitstats(unsigned long int clr, ILubyte *px)
{
	static int bitstat[32];
	static int ____count, mx_count[32];
	unsigned int c = clr & 0xffffffff, nclr;
	int i, mx, mn, diff;

	for (i = 0; c; i++) {
		if (c & 1) bitstat[i]++;
		c >>= 1;
	}
	mx = mn = 0;
	for (i = 1; i < 32; i++) {
		if (bitstat[mx] < bitstat[i])
			mx = i;
		if (bitstat[mn] > bitstat[i])
			mn = i;
	}

	/* Form new color by picking up the bits, statistically */
	nclr = 0;
	diff = (bitstat[mx] - bitstat[mn])/2;
	for (i = 0; i < 32; i++) {
		if (bitstat[mx]-bitstat[i] <= diff)
			nclr++; /* |= (1 << i); */
	}
	/* px[0] = px[1] = px[2] = mx*256/31; */
	px[0] = px[1] = px[2] = (nclr-1)*256/31;

	/*
	mx_count[mx]++;
	____count++;
	if (!(____count % 32)) {
		printf("\t");
		for (i = 0; i < 32; i++)
			printf("%3d ", mx_count[i]);
		printf("\n");
	}
	*/
}

static void compute_rgb_ones(unsigned long int clr, ILubyte *px)
{
	ILubyte w, x, y, z;
	int c = clr & 0xffffffff, v;

	w = (c & 0xff000000) >> 24;
	x = (c & 0x00ff0000) >> 16;
	y = (c & 0x0000ff00) >> 8;
	z = (c & 0x000000ff);

	v = (get_ones(w)*get_ones(x)*get_ones(y)*get_ones(z))*256/4096;
	px[0] = px[1] = px[2] = v & 0xff;
}

static void compute_rgb_csum(unsigned long int clr, ILubyte *px)
{
	
}

static void paint_random(struct codebuf *cb)
{
	ILubyte *pixels;
	int width, height, x, y, pos;
	unsigned long int clr = 0, bits_so_far = 0, cbits;

	width = height = 256;

	ilTexImage(width, height, 0, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
	pixels = malloc(sizeof(ILubyte)*3*width*height);

	//init_random(RND_ENTROPY);
	init_random(RND_PSEUDO);

#define BIT_NORMALIZE(r, mx)        (get_ones(r)*(mx)/32)
#define __nBYTE(c, n)               (((c)&0xff000000)>>(8*(n)))
#define R_NORM(c, mx)               ((__nBYTE(c, 3) ^ (c))&0xff)
#define G_NORM(c, mx)               ((((__nBYTE(c, 2) ^ (c))&0xff00)>>8)&0xff)
#define B_NORM(c, mx)               ((((__nBYTE(c, 1) ^ (c))&0xff0000)>>16)&0xff)

	for (y = 0; y < height; y++) {
	  for (x = 0; x < width; x++) {
	    pos = 3*(x + y*width);
	    clr = RND();

	    /* T1 */
	    /* pixels[pos] = pixels[pos+1] = pixels[pos+2] = BIT_NORMALIZE(clr, 0xff); */

	    /* T2: */
	    /* bits_so_far += get_ones(clr); bits_so_far = (bits_so_far * bits_so_far) % 32; */
	    /* pixels[pos] = pixels[pos+1] = pixels[pos+2] = bits_so_far*0xff/32; */

	    /* T3: */
	    /* pixels[pos] = R_NORM(clr, 0xff); pixels[pos+1] = G_NORM(clr, 0xff); pixels[pos+2] = B_NORM(clr, 0xff); */

	    /* T4: */
	    /* pixels[pos] = pixels[pos+1] = pixels[pos+2] = ((clr & 1)*0xff) & 0xff; */

	    /* T5: */
	    /* bits_so_far = 0;
	       cbits = clr;
	       do {
	       cbits = get_ones(cbits);
	       bits_so_far += cbits;
	       } while (cbits != 1);
	       pixels[pos] = pixels[pos+1] = pixels[pos+2] = (bits_so_far * 0xff)/39; */

	    /* T6: */
	    /* pixels[pos] = pixels[pos+1] = pixels[pos+2] = get_reduced_byte(clr) & 0xff; */

	    /* T7: */
	    /* compute_rgb_func1(clr, &pixels[pos]); */

	    /* T8: */
	    /* compute_rgb_bitstats(clr, &pixels[pos]); */

	    /* T9: */
	    /* compute_rgb_ones(clr, &pixels[pos]); */

	    /* S1: */
	    /* clr = (x*x + y*y); */
	    /* clr = (x*x + y*y) % (1+clr) + x*y; */
	    /* compute_rgb_func2(clr, &pixels[pos]); */

	    /* T10: */
	    compute_rgb_csum(clr, &pixels[pos]);
	  }
	}

	fini_random();

	ilSetPixels(0, 0, 0, width, height, 0, IL_RGB, IL_UNSIGNED_BYTE, pixels);
	ilSaveImage("random.png");
}

static int compile_rgb_expression(struct codebuf *cb)
{
	char *rgbexp = NULL, *code = NULL;
	size_t n, sz;

	sz = getline(&rgbexp, &n, stdin);
	if (sz == -1)
		return -1;

	code = malloc(sizeof(dyncode) + sz);
	if (!code) {
		if (rgbexp) free(rgbexp);
		return -1;
	}

	sprintf(code, dyncode, rgbexp);
	free(rgbexp);

	if (compile_snippet(cb, code))
		return -1;

	return 0;
}

int main(int argc, char *argv[])
{
	struct codebuf cb;

	if (tcc_lib_init(&cb, NULL))
		return -1;
	if (il_lib_init(&cb, 1) < 0)
		return -1;

	if (compile_rgb_expression(&cb))
		return -1;

	cb.rgb = get_sym_ref(&cb, "get_rgb");
	if (!cb.rgb)
		return -1;

	//paint_image(&cb);
	//paint_thue_morse(&cb);
	paint_random(&cb);
	release_cb(&cb);

	return 0;
}

/* compile: gcc -Wall test.c -I/home/rchandramouli/myusr/include -L/home/rchandramouli/myusr/lib -ltcc -ldl -lm -lIL */
