#ifndef BIGNUM_H
#define BIGNUM_H

#include "mytypes.h"

#ifndef MOD_BIGNUM
#define MOD_BIGNUM (10000000000000000ULL)  /* 10^16 */
#endif

#if (MOD_BIGNUM == 100000000ULL)
#define BIGPRINT_FMT    "%08I64u"
#else
#define BIGPRINT_FMT    "%016I64u"
#endif

#ifndef MAX_DIGIT_BLOCK
#define MAX_DIGIT_BLOCK  (5)
#endif

struct BigNum {
    int n;
    u64 d[MAX_DIGIT_BLOCK];
};

/* A = R1 + C */
static void BigAddConst (struct BigNum *A, struct BigNum *R1, u64 c)
{
    u64 v;
    int i;

    for (v = c, i = 0; i < R1->n; i++) {
        v += R1->d[i];
        A->d[i] = v % MOD_BIGNUM;
        v /= MOD_BIGNUM;
    }
    if (v) {
        A->n++;
        A->d[i] = v;
    }
}

/* A = R1 + R2 */
static void BigAdd (struct BigNum *A, struct BigNum *R1, struct BigNum *R2)
{
    u64 v;
    int i, n;

    n = (R1->n > R2->n) ? R2->n: R1->n;
    for (v = 0, i = 0; i < n; i++) {
        v += R1->d[i] + R2->d[i];
        A->d[i] = v % MOD_BIGNUM;
        v /= MOD_BIGNUM;
    }
    if (R1->n > n) {
        for (; i < R1->n; i++) {
            v += R1->d[i];
            A->d[i] = v % MOD_BIGNUM;
            v /= MOD_BIGNUM;
        }
    } else if (R2->n > n) {
        for (; i < R2->n; i++) {
            v += R2->d[i];
            A->d[i] = v % MOD_BIGNUM;
            v /= MOD_BIGNUM;
        }
    }
    if (v) {
        A->d[i] = v;
        i++;
    }
    A->n = i;
}

/* A = C */
static void BigInit (struct BigNum *A, u64 C)
{
    int i;

    if (C == 0) {
        A->d[0] = 0;
        A->n = 1;
    } else {
        for (i = 0; C && i < MAX_DIGIT_BLOCK; i++) {
            A->d[i] = C % MOD_BIGNUM;
            C /= MOD_BIGNUM;
        }
        A->n = i;
    }
}

/* A = R1 */
static void BigAssign (struct BigNum *A, struct BigNum *R1)
{
    int i;

    for (i = 0; i < R1->n; i++) {
        A->d[i] = R1->d[i];
    }
    A->n = R1->n;
}

/* Shift A by n words */
static int BigShift (struct BigNum *A, int n)
{
    int i;

    if (n <= 0)
        return 1;
    if (A->n > MAX_DIGIT_BLOCK-n-1)
        return 0;

    for (i = A->n+n-1; i >= n; i--)
        A->d[i] = A->d[i-n];
    for (i = n-1; i >= 0; i--)
        A->d[i] = 0;
    A->n += n;

    return 1;
}

/* A = shift(R1, n) */
static int BigAssignShifted (struct BigNum *A, struct BigNum *R1, int n)
{
    int i;

    if (n == 0) {
        BigAssign(A, R1);
        return 1;
    }
    if (R1->n > MAX_DIGIT_BLOCK-n-1)
        return 0;

    for (i = R1->n+n-1; i >= n; i--)
        A->d[i] = R1->d[i-n];
    for (i = n-1; i >= 0; i--)
        A->d[i] = 0;
    A->n = R1->n + n;

    return 1;
}


static void BigPrint (struct BigNum *A, int nblocks)
{
    int i, b;

    printf("[ (%d) ", A->n);
    b = A->n;
    if (b > nblocks) {
        for (i = b-1; i >= b-3; i--)
            printf(BIGPRINT_FMT" ", A->d[i]);
        printf(" ... ");
        for (i = 2; i >= 0; i--)
            printf(BIGPRINT_FMT" ", A->d[i]);
    } else {
        for (i = b-1; i >= 0; i--)
            printf(BIGPRINT_FMT" ", A->d[i]);
    }
    printf(" ]\n");
}

static int BigDigitSum (struct BigNum *A)
{
    int S = 0, i;
    u64 v;

    for (i = 0; i < A->n; i++) {
        v = A->d[i];
        while (v) {
            S += v % 10;
            v /= 10;
        }
    }
    return S;
}

/* A = C * R1 (quick multiply, works only for const < 100 if MOD_BIGNUM is used) */
static void BigMultiplyConst (struct BigNum *A, struct BigNum *R1, u64 C)
{
    u64 v;
    int i;

    for (v = 0, i = 0; i < R1->n; i++) {
        v += R1->d[i] * C;
        A->d[i] = v % MOD_BIGNUM;
        v /= MOD_BIGNUM;
    }
    while (v) {
        A->d[i++] = v % MOD_BIGNUM;
        v /= MOD_BIGNUM;
    }
    A->n = i;
}

/* A = B * C */
static void BigMultiply (struct BigNum *A, struct BigNum *B, struct BigNum *C)
{
    struct BigNum t1, t2;
    int i;

    BigInit(&t1, 0);
    BigInit(&t2, 0);

    for (i = 0; i < C->n; i++) {
        BigAssignShifted(&t1, B, i);
        BigMultiplyConst(&t1, &t1, C->d[i]);
        BigAdd(&t2, &t2, &t1);
    }
    BigAssign(A, &t2);
}

/* Is A == B */
static int BigEqual (struct BigNum *A, struct BigNum *B)
{
    int i;

    if (A->n != B->n)
        return 0;

    for (i = 0; i < A->n; i++)
        if (A->d[i] != B->d[i])
            return 0;

    return 1;
}

/* Return: (0: A == B, -1: A < B, 1: A > B) */
static int BigCompare (struct BigNum *A, struct BigNum *B)
{
    int i;

    if (A->n > B->n) {
        return 1;
    } else if (A->n < B->n) {
        return -1;
    } else {
        for (i = A->n-1; i >= 0; i--) {
            if (A->d[i] > B->d[i])
                return 1;
            else if (A->d[i] < B->d[i])
                return -1;
        }
    }
    return 0;
}

#endif /* BIGNUM_H */
