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

#ifndef zenic_ps2_Vu0_h
#define zenic_ps2_Vu0_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Dma;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VU0_MICRO ((volatile u32 *)(0x11000000))
#define VU0_MEM   ((volatile u32 *)(0x11004000))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Vu0
{
public:

	enum User
	{
		StandardMath
	};

	enum StandardMathOffsets
	{
		MatrixMultiply1 = 0
	};


	static void init();

	static void setUser(User user, bool upload = true, bool waitFinised = true);
	static void dumpOnce(const char* microMem = "host:vu0micromem", const char* mem = "host:vu0mem");
	static User user();

	// Static Math functions

private:

	static uint calcSize(const u32* start, const u32* end);
	static void addToChain(Dma& dmaChain, u32* start, u32* end);


	static User m_user;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Vu0::User Vu0::user()
{
	return m_user;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif

