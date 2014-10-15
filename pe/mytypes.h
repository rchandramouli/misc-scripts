#ifndef MYTYPES_H
#define MYTYPES_H

typedef unsigned long int u32;
typedef unsigned long long int u64;

typedef long int s32;
typedef long long int s64;

#ifdef _WIN32
#define FMT_S64  "%I64d"
#define FMT_U64  "%I64u"
#else
#define FMT_S64  "%Ld"
#define FMT_U64  "%Lu"
#endif

#endif /* MYTYPES_H */
