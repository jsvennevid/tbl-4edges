///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_ps2_types_h
#define zenic_ps2_types_h

typedef float f32;

typedef unsigned char u8;
typedef signed char s8;
typedef char c8;

typedef unsigned short u16;
typedef short s16;

typedef unsigned int u32;
typedef int s32;

typedef unsigned long u64;
typedef long s64;

typedef unsigned int uint;

#define ZENIC_RESTRICT __restrict
#define ALIGN(x) __attribute__((aligned(x)))

#define _TAMTYPES_H_ 1
#define TYPES_DEFINED

#endif //zenic_ps2_types_h
