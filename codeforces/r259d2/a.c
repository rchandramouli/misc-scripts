#include <stdio.h>

int main (int argc, char *argv[])
{
    char ch[102];
    int n, i, s, t;

    scanf("%d", &n);

    for (i = 0; i < n; i++)
        ch[i] = '*';
    ch[n>>1] = 'D';
    ch[n] = '\0';

    t = i = n >> 1;
    s = -1;

    while (n--) {
        printf("%s\n", ch);

        i += s;
        ch[i] = 'D' + ((1+s)>>1)*('*' - 'D');
        ch[(t << 1)-i] = ch[i];

        if (t == n-1) {
            s = 1;
            --i;
        }
    }

    return 0;
}
