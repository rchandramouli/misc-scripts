#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TOKENS        (16)
#define MAX_TOKEN_NODES   (1024)

typedef struct _tokn_s {
    int n;
    uint32_t pfx[MAX_TOKENS];
    uint32_t sfx2[MAX_TOKENS];
} tokn_t;

typedef tokn_t* toknlist_t[MAX_TOKEN_NODES];

#define tokn_sfx2(v, n)       (v)->sfx2[n]
#define tokn_pfx(v, n)        (v)->pfx[n]

/* v = x */
#define tokn_copy(v, x)       memcpy(v, x, sizeof(tokn_t))

/* v->pfx = { 0 } */
#define tokn_pfx_reset(v)     memset((v)->pfx, 0, sizeof((v)->pfx))

static bool tokn_is_zero (tokn_t *v, bool pfx, bool sfx2)
{
    int i;

    if (pfx) {
	for (i = 0; i < v->n; i++) {
	    if (tokn_pfx(v, i))
		return false;
	}
    }

    if (sfx2) {
	for (i = 0; i < v->n; i++) {
	    if (tokn_sfx2(v, i))
		return false;
	}
    }

    return true;
}

static bool tokn_is_equal (tokn_t *v1, tokn_t *v2, bool pfx, bool sfx2)
{
    int i;

    if (v1->n != v2->n) {
	return false;
    }

    if (pfx) {
	for (i = 0; i < v1->n; i++) {
	    if (tokn_pfx(v1, i) != tokn_pfx(v2, i))
		return false;
	}
    }

    if (sfx2) {
	for (i = 0; i < v1->n; i++) {
	    if (tokn_sfx2(v1, i) != tokn_sfx2(v2, i))
		return false;
	}
    }

    return true;
}

static inline void tokn_init (tokn_t *v, int n)
{
    assert(v);

    memset(v, 0, sizeof(tokn_t));
    v->n = n;
}

static inline tokn_t *tokn_alloc (int n)
{
    tokn_t *v = malloc(sizeof(tokn_t));

    tokn_init(v, n);
    return v;
}

static inline void toknlist_free (toknlist_t list)
{
    int i;

    for (i = 0; i < MAX_TOKEN_NODES; i++) {
	if (list[i]) {
	    free(list[i]);
	    list[i] = NULL;
	}
    }
}

#define toknlist_init(L)   memset(L, 0, sizeof(toknlist_t))

/* v = x*y */
static void tokn_multiply (tokn_t *v, tokn_t *x, tokn_t *y)
{
    int i;

    assert(x->n == y->n);

    tokn_init(v, x->n);

    for (i = 0; i < x->n; i++) {
	tokn_sfx2(v, i) = tokn_sfx2(x, i) ^ tokn_sfx2(y, i);
	tokn_pfx(v, i) = tokn_pfx(x, i) + tokn_pfx(y, i);
	tokn_pfx(v, i) += tokn_sfx2(x, i) & tokn_sfx2(y, i);
    }
}

static tokn_t *toknlist_find (toknlist_t l, tokn_t *x, bool *is_pfx, bool *is_sfx)
{
    int i;

    for (i = 0; l[i]; i++) {
	if (l[i]->n != x->n)
	    continue;
	if (tokn_is_equal(l[i], x, false, true)) {
	    *is_sfx = true;
	    if (tokn_is_equal(l[i], x, true, false))
		*is_pfx = true;
	    return l[i];
	}
    }
    return NULL;
}

static bool toknlist_compare (toknlist_t l1, toknlist_t l2)
{
    int i;
    bool _sfx, _pfx;

    for (i = 0; l1[i] && l2[i]; i++);
    if (l1[i] || l2[i])
	return false;

    for (i = 0; l1[i]; i++) {
	_sfx = _pfx = false;
	toknlist_find(l2, l1[i], &_pfx, &_sfx);
	if (!_pfx || !_sfx)
	    return false;
    }

    return true;
}

static void tokn_dump (tokn_t *x)
{
    int i;

    for (i = 0; i < x->n; i++) {
	if (tokn_pfx(x, i) > 1) {
	    printf("%c^%d", 'A'+i, tokn_pfx(x, i));
	} else if (tokn_pfx(x, i)) {
	    printf("%c", 'A'+i);
	}
    }
    printf("#");
    for (i = 0; i < x->n; i++) {
	if (tokn_sfx2(x, i)) {
	    printf("%c", 'A'+i);
	}
    }
}

static void toknlist_dump (toknlist_t l)
{
    int i;

    for (i = 0; l[i]; i++) {
	tokn_dump(l[i]);
	printf(", ");
    }
    printf("\n");
}

/* For all token nodes, check if the pfx values are same, if so, reset
 * pfx values. Remove all nodes that do not have sfx2 set.
 */
static void toknlist_normalize (toknlist_t l, bool do_check)
{
    int i;
    tokn_t t;

    if (do_check) {
	tokn_copy(&t, l[0]);
	for (i = 1; l[i]; i++) {
	    if (!tokn_is_equal(l[i], &t, true, false))
		return;
	}
    }

    for (i = 0; l[i]; i++) {
	tokn_pfx_reset(l[i]);
    }
}

/* V = l*l */
static void toknlist_square (toknlist_t V, toknlist_t l)
{
    int i, j, k = 0;
    tokn_t *x = NULL;
    tokn_t *f;
    bool _sfx, _pfx;

    for (i = 0; l[i]; i++) {
	for (j = i+1; l[j]; j++) {
	    _sfx = _pfx = false;
	    if (x == NULL) {
		x = tokn_alloc(l[i]->n);
	    }
	    tokn_multiply(x, l[i], l[j]);
	    /* Skip if product has no sfx2 */
	    if (tokn_is_zero(x, false, true))
		continue;

	    f = toknlist_find(V, x, &_pfx, &_sfx);
	    if (!f || !_sfx) { /* RTN: (f && _sfx && !_pfx) */
		V[k++] = x;
		x = NULL;
	    }
	}
    }

    toknlist_normalize(V, false); /* RTN: true */
}

static void tokn_iterator_3 (void)
{
    tokn_t x[3];
    toknlist_t L[4];
    int i;

    for (i = 0; i < 3; i++) {
	tokn_init(&x[i], 3);
	tokn_sfx2(&x[i], i) = 1;
	toknlist_init(L[i]);
	L[0][i] = &x[i];
    }

    toknlist_dump(L[0]);

    for (i = 0; i < 2; i++) {
	toknlist_square(L[i+1], L[i]);
	toknlist_dump(L[i+1]);
    }
    for (i = 0; i < 2; i++) {
	toknlist_free(L[i+1]);
    }
}

static void tokn_iterator_4 (void)
{
    tokn_t x[4];
    toknlist_t L[5];
    int i;

    for (i = 0; i < 4; i++) {
	tokn_init(&x[i], 4);
	tokn_sfx2(&x[i], i) = 1;
	toknlist_init(L[i]);
	L[0][i] = &x[i];
    }

    toknlist_dump(L[0]);

    for (i = 0; i < 3; i++) {
	toknlist_init(L[i+1]);
	toknlist_square(L[i+1], L[i]);
	toknlist_dump(L[i+1]);
    }

    for (i = 0; i < 3; i++) {
	toknlist_free(L[i+1]);
    }
}

static void tokn_iterator_6 (void)
{
    tokn_t x[6];
    toknlist_t L[7];
    int i;

    for (i = 0; i < 6; i++) {
	tokn_init(&x[i], 6);
	tokn_sfx2(&x[i], i) = 1;
	toknlist_init(L[i]);
	L[0][i] = &x[i];
    }

    toknlist_dump(L[0]);

    for (i = 0; i < 5; i++) {
	toknlist_init(L[i+1]);
	toknlist_square(L[i+1], L[i]);
	toknlist_dump(L[i+1]);
    }

    for (i = 0; i < 5; i++) {
	toknlist_free(L[i+1]);
    }
}

int main (int argc, char *argv[])
{
    printf("3-token square-root iterator:\n");
    tokn_iterator_3();

    printf("\n4-token square-root iterator:\n");
    tokn_iterator_4();

    printf("\n6-token square-root iterator:\n");
    tokn_iterator_6();

    return 0;
}

