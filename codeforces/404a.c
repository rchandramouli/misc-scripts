#include <stdio.h>

int main (int argc, char *argv[])
{
    int n, i, j, k, ch[26];
    char s[300], dch;
    int found = 0, diag = 0;

    scanf("%d", &n);

    for (i = 0; i < 26; i++)
        ch[i] = 0;

    dch = '\0';
    i = 0;
    k = n;

    while (n--) {
        scanf("%s", s);

        if (dch == '\0')
            dch = s[0];

        if (s[i] != s[n] || s[i] != dch || s[n] != dch)
            break;

        for (j = 0; j < k; j++)
            ch[s[j]-'a']++;
        i++;
    }

    k = 2*k - (k&1);

    if (n < 0) {
        for (i = 0; i < 26; i++) {
            if (ch[i]) found++;

            if (ch[i] == k)
                diag = 1;
        }
    }

    printf("%s\n", (found == 2 && diag) ? "YES": "NO");

    return 0;
}
