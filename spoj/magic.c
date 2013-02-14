#include <stdio.h>
#define A 13456ULL
#define B 3137ULL
#define C 65535ULL
int main (int a, char *_[]){
long long int s=A, i;
for(i=0;i<10000;i++)printf("%lld\n", s=(s*B+i)%C);
return 0;
}
