#include <stdio.h>

typedef unsigned long long int u64;

static int nhash[100][10];
static int count[100], all_count = 0;

static void build_hash_table (void)
{
    int i, j, k;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            for (k = 0; k < 10; k++) {
                if (i + j + k > 9)
                    continue;
                nhash[10*i+j][count[10*i+j]] = k;
                count[10*i+j]++;
                all_count++;
            }
        }
    }
}

static u64 n_count = 0;
static int d; /* 3+2 = 5 digits */

static void count_valid_numbers (int n, int c)
{
    int i;

    if (c == d) {
        n_count++;
        if ((n_count % 1000000000ULL) == 0)
            printf("\tcount = %Lu\n", n_count);
        return;
    }
    n %= 100;
    for (i = 0; i < count[n]; i++) {
        count_valid_numbers(10*n + nhash[n][i], c+1);
    }
}

int main (int argc, char *argv[])
{
    int i, j;

    scanf("%d", &d);
    d -= 2;

    build_hash_table();
    printf("Number of 3 digit numbers = %d\n", all_count);

    for (i = 10; i < 100; i++) {
        if (!count[i]) continue;

        for (j = 0; j < count[i]; j++) {
            count_valid_numbers(10*i+nhash[i][j], 1);
        }
    }
    printf("%Lu\n", n_count);

    return 0;
}
