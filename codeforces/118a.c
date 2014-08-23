#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[])
{
    char s1[200], s2[200], *p1, *p2;

    scanf("%s", s1);

    p1 = s1;
    p2 = s2;

    while (*p1) {
        switch (*p1) {
        case 'A':
        case 'a':
        case 'E':
        case 'e':
        case 'I':
        case 'i':
        case 'o':
        case 'O':
        case 'u':
        case 'U':
        case 'y':
        case 'Y':
            p1++;
            continue;
        }
        *p2 = '.';
        p2++;

        if (*p1 >= 'A' && *p1 <= 'Z')
            *p2 = *p1 - 'A' + 'a';
        else
            *p2 = *p1;
        p2++;
        p1++;
    }
    *p2 = '\0';

    printf("%s\n", s2);

    return 0;
}
