#include <stdio.h>

int main (int argc, char *argv[])
{
    unsigned long long int t, f, p, z;

    scanf("%llu", &t);
    while (t--) {
        scanf("%llu", &f);
        z = 0; p = 1;

        do {
            p *= 5;
            z += f/p;
        } while (f > p);
        printf("%llu\n", z);
    }
    return 0;
}

