///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


