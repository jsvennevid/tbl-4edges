///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_FileSystem_h
#define zenic_FileSystem_h

#if defined(ZENIC_WIN32)
	#include "Win32/FileSystem.h"
#elif defined(ZENIC_PS2)
	#include "Ps2/FileSystem.h"
#else
	#error "Unsupported platform"
#endif

#endif

