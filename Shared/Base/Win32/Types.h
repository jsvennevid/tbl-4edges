/*

Copyright (c) 2004-2006 Jesper Svennevid, Daniel Collin

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
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

#define ZENIC_PACKED // __attribute__((packed))
#define ZENIC_ALIGN(x) //__attribute__((aligned(x)))

typedef unsigned int uint;

#endif
