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

#include "AssertDisplay.h"
#ifndef PS2_EMU
#include <debug.h>
#endif
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef PS2_EMU
void AssertDisplay::callback(const char*, const char*, uint)
#else
void AssertDisplay::callback(const char* expression, const char* file, uint line)
#endif
{

	static char tempBuffer[2048];

#ifndef PS2_EMU
	init_scr();

	// show callstack
/*
	static unsigned int* stackTrace[128];

	scr_printf("Callstack\n");

	ps2GetStackTrace((unsigned int*)&stackTrace, 128);
	
	for (uint i = 0; i < 128; ++i)
	{
		if (stackTrace[i] == 0)
			break;
	
		scr_printf("adress 0x%08x\n", (int)stackTrace[i]);
	}
*/
	// assert error

	sprintf(tempBuffer, "Assert at %s(%d) | %s\n", file, line, expression);
	scr_printf("%s", tempBuffer);

	while (1)
	{
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}

