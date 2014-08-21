#include <stdio.h>
#include <gmp.h>

static unsigned int get_digit_sum (mpz_t n)
{
    mpz_t q;
    unsigned int s = 0;

    mpz_init(q);

    while (mpz_cmp_ui(n, 0)) {
        s += mpz_tdiv_q_ui(q, n, 10);
        mpz_set(n, q);
    }
    return s;
}

int main (int argc, char *argv[])
{
    unsigned int a, b, s;
    unsigned int ma, mb, max = 0;
    mpz_t p;

    mpz_init(p);

    for (a = 1; a < 100; a++) {
        for (b = 1; b < 100; b++) {
            mpz_ui_pow_ui(p, a, b);

            s = get_digit_sum(p);
            if (max < s) {
                max = s;
                ma = a;
                mb = b;
            }
        }
    }
    printf("ma = %u, mb = %u, max = %u\n", ma, mb, max);

    return 0;
}
