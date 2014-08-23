#include <stdio.h>

typedef unsigned long long int u64;

static int place[64], rev[64];
static int max_c, b, mask;

static inline int rotate (int x)
{
    return (((x & mask) << 1) | ((x >> b) & 1));
}

static inline int rotate_xor (int x)
{
    return (((x & mask) << 1) | ((~x >> b) & 1));
}

static inline void unvisit (int n)
{
    rev[place[n]-1] = 0;
    place[n] = 0;
}

static inline void visit (int n, int r)
{
    place[n] = r;
    rev[place[n]-1] = n;
}

static inline int visited (int n)
{
    return place[n];
}

static char *get_binary (int n, int b)
{
    static char binstr[64];

    binstr[b] = 0;
    while (b--) {
        binstr[b] = '0' + (n & 1);
        n >>= 1;
    }
    return binstr;
}

static void print_visits (void)
{
    int i;

    for (i = 0; i < max_c; i++) {
        printf("%s ", get_binary(rev[i], b));
    }
    printf("\n");
}

static u64 get_visit_signature (void)
{
    u64 signature = 0;
    int i;

    for (i = 1; i < max_c-b+1; i++) {
        signature <<= 1;
        signature |= rev[i] & 1;
    }
    return signature;
}

static u64 sig_sum = 0, sig_count = 0;

static void visit_circle (int n, int r)
{
    int n1;

    if (visited(n))
        return;

    visit(n, r);

    if (r == max_c) {
        //print_visits();
        u64 s = get_visit_signature();

        sig_count++;
        sig_sum += s;
        //printf("Signature = %Lu (binary = %s)\n", s, get_binary(s, 64));
        unvisit(n);
        return;
    }

    n1 = rotate(n);
    if (!visited(n1)) {
        visit_circle(n1, r+1);
    }

    n1 ^= 1;
    if (!visited(n1)) {
        visit_circle(n1, r+1);
    }

    unvisit(n);
}

int main (int argc, char *argv[])
{
    scanf("%d", &b);

    max_c = (1 << b);
    mask = (max_c >> 1) - 1;

    visit_circle(0, 1);

    printf("\nTotal signature sum = %Lu (counts = %Lu)\n", sig_sum, sig_count);

    return 0;
}
