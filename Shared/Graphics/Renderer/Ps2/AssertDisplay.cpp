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

