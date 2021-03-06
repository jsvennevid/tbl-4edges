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

#ifndef zenic_win32_Assert_h
#define zenic_win32_Assert_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../Types.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forwards
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class BufferStream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

class Assert
{
public:

	enum
	{
		Requester       = 0x01,
		ErrorStream     = 0x02,
		DebugOutput     = 0x04
	};

	static void assertStatement(const char* expression, const char* filename, uint line);

	static void setMethods(uint methods); 
	static uint methods();

private:

	enum
	{
		LineLength = 60
	};

	static void truncate(BufferStream& s, uint length, const char* input);

	static uint ms_methods;
};

}

#endif
