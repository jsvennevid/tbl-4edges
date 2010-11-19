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

#ifndef zenic_Prefetch_h
#define zenic_Prefetch_h

#if defined(ZENIC_WIN32)
#include <xmmintrin.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(ZENIC_WIN32)

#define ZENIC_PREFETCH_READ(address) _mm_prefetch((char*)address, _MM_HINT_T0);
#define ZENIC_PREFETCH_WRITE(address) _mm_prefetch((char*)address, _MM_HINT_T0);

#elif defined(ZENIC_PS2)

#define ZENIC_PREFETCH_READ(address) asm __volatile__("pref 0, 0(%0)" : : "r" (address));
#define ZENIC_PREFETCH_WRITE(address) asm __volatile__("pref 0, 0(%0)" : : "r" (address));

#elif defined(ZENIC_PSP)

#define ZENIC_PREFETCH_READ(address)
#define ZENIC_PREFETCH_WRITE(address)

#else

#define ZENIC_PREFETCH_READ(address)
#define ZENIC_PREFETCH_WRITE(address)

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif


