#include <stdio.h>
typedef unsigned long long int U;
int main () {
U t, n, k, c, a, s;
scanf("%llu", &t);
while (t--) {
scanf("%llu", &n);
k = n>>2;
c = n&3;
a = n*(((k*k)<<3)+((k*c)<<2)+k)+((n*(c+2+((c*c)<<1)))>>2);
s = a+n*n*(n+((n+1)<<1));
printf("%llu\n",s);
}
return 0;
}
