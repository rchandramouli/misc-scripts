#include <stdio.h>

int main (int argc, char *argv[])
{
    int w;

    scanf("%d", &w);

    printf("%s\n", (w > 2) ? ((w & 1) ? "NO": "YES"): "NO");

    return 0;
}
