///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_Assert_h
#define zenic_Assert_h

#if !defined(ZENIC_FINAL)

#if defined(ZENIC_WIN32)
	#include "Win32/Assert.h"
#elif defined(ZENIC_PS2)
	#include "Ps2/Assert.h"
#elif defined(ZENIC_PSP)
	#include "Psp/Assert.h"
#else
	#error "Unsupported platform"
#endif

#define ZENIC_ASSERT(exp) (void)((exp) || ( zenic::Assert::assertStatement(#exp, __FILE__, __LINE__), 0 ))
#define ZENIC_ASSERT_DESC(exp,desc) (void)((exp) || ( zenic::Assert::assertStatement(desc, __FILE__, __LINE__), 0 ))

#else

#define ZENIC_ASSERT(exp) ((void)0)
#define ZENIC_ASSERT_DESC(exp,desc) ((void)0)

#endif

#endif
