#include <stdio.h>

int main (int argc, char *argv[])
{
    int n, a, b;
    int found = 0;

    scanf("%d", &n);
    while (n--) {
        scanf("%d %d", &a, &b);
        if (b < a) {
            found = 1;
            break;
        }
    }
    if (found) {
        printf("Happy Alex\n");
    } else {
        printf("Poor Alex\n");
    }
    return 0;
}
