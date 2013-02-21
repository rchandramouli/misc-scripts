//#include <stdio.h>
#ifndef TLOOP_H
#define TLOOP_H

#define F(v,m,i) for(v=i; v<(m);v++)

#ifdef FMX_BASE0
#  define CONS        1
#else
#  define CONS        0
#endif

#ifdef MXVAL
#  define VAL       (MXVAL+CONS)
#else
#  define VAL       (MX+CONS)
#endif

#define FMX(t) F(l[t],VAL,1)

#define FMX_1  FMX(0)
#define FMX_2  FMX_1 FMX(1)
#define FMX_3  FMX_2 FMX(2)
#define FMX_4  FMX_3 FMX(3)
#define FMX_5  FMX_4 FMX(4)
#define FMX_6  FMX_5 FMX(5)
#define FMX_7  FMX_6 FMX(6)
#define FMX_8  FMX_7 FMX(7)
#define FMX_9  FMX_8 FMX(8)
#define FMX_10 FMX_9 FMX(9)

#ifndef MX
#  define FMX_N   while(0)
#else
#if MX==1
#  define FMX_N FMX_1
#elif MX==2
#  define FMX_N FMX_2
#elif MX==3
#  define FMX_N FMX_3
#elif MX==4
#  define FMX_N FMX_4
#elif MX==5
#  define FMX_N FMX_5
#elif MX==6
#  define FMX_N FMX_6
#elif MX==7
#  define FMX_N FMX_7
#elif MX==8
#  define FMX_N FMX_8
#elif MX==9
#  define FMX_N FMX_9
#elif MX==10
#  define FMX_N FMX_10
#endif
#endif

#endif /* TLOOP_H */
