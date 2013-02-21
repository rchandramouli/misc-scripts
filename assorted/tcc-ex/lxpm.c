#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct bt {
	struct bt *left, *right;
};

static inline void init_bt(struct bt *head)
{
	head->left = head->right = head;
}

static struct bt *find_bt(struct bt *head, struct bt *key,
			  int (*cmpfn)(struct bt *, struct bt *))
{
	struct bt *node = head, *prev;
	int res;

	while (1) {
		res = cmpfn(node, key);
		prev = node;

		if (res == 0) {
			return node;
		} else if (res > 0) {
			node = node->left;
		} else {
			node = node->right;
		}

		if (prev == node)
			break;
	}

	return NULL;
}

static struct bt *find_and_insert_bt(struct bt *head, struct bt *key,
				     int (*cmpfn)(struct bt *, struct bt *))
{
	struct bt *node = head;
	int res;

	while (1) {
		res = cmpfn(node, key);
		if (res == 0)
			return node;

		if (res > 0) {
			if (node->left == node) {
				node->left = key;
				break;
			}
			node = node->left;
		} else {
			if (node->right == node) {
				node->right = key;
				break;
			}
			node = node->right;
		}
	}

	return key;
}

static inline int count_bt(struct bt *head)
{
	int l = 0, r = 0;

	if (head->left == head && head->right == head)
		return 1;

	if (head->left != head)
		l = count_bt(head->left);
	if (head->right != head)
		r = count_bt(head->right);

	return (1 + l + r);
}

/* pixel */
struct pixel {
	char sym[2];
	unsigned char rgb[3];

	struct bt tree;
};


#define offset_of(type, member) (unsigned long)(&((type *)0)->member)
#define container_of(ptr, type, member)					\
        (type *)((unsigned long)(ptr) - offset_of(type, member))
#define bt_to_pixel(bt)     container_of(bt, struct pixel, tree)

#define __gt_int(x1, x2)    (((x1) > (x2)) ? 1: ((x1) == (x2)) ? 0: -1)

static struct pixel *pixel_head;
static unsigned char **screen;

static unsigned int symchars = 0, symidx = 0, symcount, pix_wd, pix_ht;
static char syms[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*.";

static int cmp_pixel(struct bt *bt1, struct bt *bt2)
{
	struct pixel *px1 = bt_to_pixel(bt1), *px2 = bt_to_pixel(bt2);
	int cmp = 0;

	if ((cmp = __gt_int(px1->rgb[0], px2->rgb[0])))
		return cmp;

	if ((cmp = __gt_int(px1->rgb[1], px2->rgb[1])))
		return cmp;

	if ((cmp = __gt_int(px1->rgb[2], px2->rgb[2])))
		return cmp;

	return 0;
}

static void get_next_symbol(char *sym)
{
	unsigned int l = symidx;

	sym[0] = syms[l % symcount];
	l /= symcount;
	if (symchars > 0) {
		sym[1] = syms[l % symcount];
		l /= symcount;
	}
	if (symchars > 1) {
		sym[2] = syms[l % symcount];
	}
	symidx++;
}

static void assign_symbol_to_one(struct bt *bt)
{
	struct pixel *px = bt_to_pixel(bt);
	char sym[2];

	get_next_symbol(sym);
	px->sym[0] = sym[0];
	px->sym[1] = sym[1];
}

static void assign_symbol_to_all(struct bt *bt)
{
	assign_symbol_to_one(bt);

	if (bt->left != bt)
		assign_symbol_to_all(bt->left);

	if (bt->right != bt)
		assign_symbol_to_all(bt->right);
}

static int add_color(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	static struct pixel *pixel = NULL;
	struct pixel *px = NULL;
	struct bt *bt;
	int t;

	if (!pixel) {
		pixel = calloc(sizeof(struct pixel), 1);
		if (!pixel) {
			return -1;
		}
	}

	pixel->rgb[0] = r;
	pixel->rgb[1] = g;
	pixel->rgb[2] = b;

	init_bt(&pixel->tree);

	if (!pixel_head) {
		pixel_head = pixel;
	} else {
		bt = find_and_insert_bt(&pixel_head->tree, &pixel->tree, cmp_pixel);
		if (bt != &pixel->tree) {
			px = bt_to_pixel(bt);
		}
	}
	if (!px) {
		assign_symbol_to_one(&pixel->tree);
		px = pixel;
		pixel = NULL;
	}

	/* add to the color matrix, the symbol */
	for (t = 0; t < symchars; t++)
		screen[y][symchars*x+t] = px->sym[t];

	return 0;
}

static int color_init(int wd, int ht, int chars)
{
	int i, res = 0;

	symcount = sizeof(syms)/sizeof(char);
	symchars = chars;
	pix_wd = wd;
	pix_ht = ht;

	screen = (unsigned char **)malloc(sizeof(char *) * ht);
	if (!screen) return -1;

	for (i = 0; i < ht; i++) {
		screen[i] = (unsigned char *)malloc(sizeof(char)*symchars*wd);
		if (!screen[i])
			goto __free_exit;
	}

	return res;

__free_exit:
	while (--i >= 0) {
		free(screen[i]);
	}
	free(screen);

	return -1;
}

static inline void dump_xpm_color(struct pixel *px)
{
	int i;

	printf("\"");
	for (i = 0; i < symchars; i++)
		printf("%c", px->sym[i]);
	printf(" c #%02x%02x%02x\",\n", px->rgb[0], px->rgb[1], px->rgb[2]);
}

static void dump_xpm_color_table(struct bt *bt)
{
	struct pixel *px = bt_to_pixel(bt);

	if (!bt) return;
	dump_xpm_color(px);

	if (bt->left != bt)
		dump_xpm_color_table(bt->left);
	if (bt->right != bt)
		dump_xpm_color_table(bt->right);
}

static void dump_xpm(void)
{
	int x, y, t, c;

	c = count_bt(&pixel_head->tree);

	printf("/* XPM */\nstatic char *myimg_xpm[] = {\n");
	printf("\"%d %d %d %d\",\n", pix_wd, pix_ht, c, symchars);

	/* color table */
	dump_xpm_color_table(&pixel_head->tree);

	/* pixel table */
	for (y = 0; y < pix_ht; y++) {
		printf("\"");
		for (x = 0; x < pix_wd; x++) {
			for (t = 0; t < symchars; t++)
				printf("%c", screen[y][symchars*x+t]);
		}
		printf("\",\n");
	}

	printf("}\n");
}

/*------------------------------------- Pattern stuff ------------------------*/
static int ones[256];

static int get_ones(unsigned int i)
{
	int c = 0;

	while (i) {
		c += (i & 1);
		i >>= 1;
	}

	return c;
}

static int get_all_ones(unsigned int l)
{
	int c = 0;

	while (l) {
		c += ones[l & 0xff];
		l >>= 8;
	}

	return c;
}

static void precompute(void)
{
	int i;

	for (i = 0; i < 256; i++)
		ones[i] = get_ones(i);
}

int main(int argc, char *argv[])
{
	int x = 0, y = 0, n, res;
	unsigned int l;

	scanf("%d", &n);

	precompute();

	color_init(n, n, 2);

	for (x = 0; x < n; x++) {
		for (y = 0; y < n; y++) {
			l = get_all_ones((n-x)*x+(n-y)*y);

			/* l would be <= 32, so, pump it to be within 256 */
			l = (l * 8) & 0xff;

			/* add it to pallette */
			res = add_color(x, y, l & 0xff, l & 0xff, l & 0xff);
			if (res < 0) {
				return -1;
			}
		}
	}

	dump_xpm();

	return 0;
}
