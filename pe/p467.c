/* TBD: Super integer */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mytypes.h"
#include "timeit.h"

#define PTYPE_T_DEFINED
#define ptype_t u32
#define MAX_PTABLE_SIZE     (50001)
#include "prime.h"

#define MAX_STRING_LENGTH   (10000)

static char pd[MAX_PTABLE_SIZE], cd[MAX_PTABLE_SIZE];
static char sup[MAX_STRING_LENGTH];

static int get_lcs_length (int len)
{
    int len_x[MAX_STRING_LENGTH], len_x1[MAX_STRING_LENGTH];

    
}

int main (int argc, char *argv[])
{
    int i, j, c;

    timeit_timer_start();
    generate_primes();
    timeit_timer_peek_and_print();

    /* Pick digits */
    for (i = 0; i < 10000; i++) {
        pd[i] = ptable[i] % 9;
        if (!pd[i])
            pd[i] = 9;
    }
    for (c = 0, i = 1; c < 10000; i++) {
        for (j = ptable[i]+1; j < ptable[i+1]; j++, c++) {
            cd[c] = j % 9;
            if (!cd[c])
                cd[c] = 9;
        }
    }
    timeit_timer_peek_and_print();

    /* Print first 10 numbers */
    for (i = 0; i < 10; i++) {
        printf("%c", '0'+pd[i]);
    }
    printf("\n");
    for (i = 0; i < 10; i++) {
        printf("%c", '0'+cd[i]);
    }

    return 0;
}
