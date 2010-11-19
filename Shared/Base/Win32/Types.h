#ifndef zenic_win32_Types_h
#define zenic_win32_Types_h

#if defined(__GNUC__)
#include <stdint.h>
#endif

typedef float f32;

typedef unsigned char u8;
typedef signed char s8;
typedef char c8;

typedef unsigned short u16;
typedef short s16;

typedef unsigned int u32;
typedef int s32;

#if defined(__GNUC__)
typedef uint64_t u64;
typedef int64_t s64;
#else
typedef unsigned __int64 u64;
typedef __int64 s64;
#endif

#define ZENIC_RESTRICT __restrict
#define ALIGN(x)

typedef unsigned int uint;

#endif
