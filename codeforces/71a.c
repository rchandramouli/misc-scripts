#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[])
{
    int n, l;
    char s[200];

    scanf("%d", &n);

    while (n--) {
        scanf("%s", s);
        l = strlen(s)-2;

        if (l > 8) {
            printf("%c%d%c\n", s[0], l, s[l+1]);
        } else {
            printf("%s\n", s);
        }
    }

    return 0;
}
