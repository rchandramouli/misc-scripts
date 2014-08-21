#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[])
{
    int n, l;
    char val[100001];

    scanf("%s", val);
    l = strlen(val);

    n = val[l-1]-'0';
    if (l > 1) {
        n += 10*(val[l-2]-'0');
    }

    if (n & 1) {
        printf("0\n");
    } else if ((n % 4) == 0) {
        printf("4\n");
    } else if ((n % 4) == 2) {
        printf("0\n");
    } else {
        printf("%d\n", (n % 4));
    }
    return 0;
}
