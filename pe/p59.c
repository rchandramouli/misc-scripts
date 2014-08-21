#include <stdio.h>
#include <ctype.h>

static char key[3] = { 'a', 'a', 'a' };

static int get_next_key (void)
{
    if (key[0] == 'z' && key[1] == 'z' && key[2] == 'z')
        return 0;

    key[2]++;
    if (key[2] > 'z') {
        key[2] = 'a';
        key[1]++;
    }
    if (key[1] > 'z') {
        key[1] = 'a';
        key[0]++;
    }
    if (key[0] > 'z') {
        key[0] = 'a';
    }
    return 1;
}

int main (int argc, char *argv[])
{
    int v, i, s, l, flg;
    char buf[2048];
    char decode[32];

    i = 0;
    while (scanf("%d,", &v) == 1) {
        buf[i++] = v;
    }
    l = i;
    buf[i] = 0;

    do {
        flg = 1;
        for (i = 0; i < 32; i++) {
            decode[i] = buf[i] ^ key[i % 3];
            if (!isprint(decode[i]) || 0) {
                //!(isalnum(decode[i]) || isspace(decode[i]) || ispunct(decode[i]))) {
                flg = 0;
                break;
            }
        }
        if (!isalpha(decode[0]) && decode[0] != '\'' && decode[0] != '"' && decode[0] != '(')
            flg = 0;

        if (flg) {
            decode[i-1] = 0;
            s = 0;
            for (i = 0; i < l; i++)
                s += buf[i] ^ key[i % 3];

            printf("KEY [%c%c%c - %d] [[ %s ]]\n",
                   key[0], key[1], key[2], s, decode);
        }
    } while (get_next_key());

    return 0;
}
