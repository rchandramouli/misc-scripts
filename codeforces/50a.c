#include <stdio.h>

int main (int argc, char *argv[])
{
    int m, n;

    scanf("%d %d", &m, &n);
    printf("%d\n", (m*n)>>1);

    return 0;
}
