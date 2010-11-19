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

#ifndef zenic_ps2_hardware_DmaSimulator_h
#define zenic_ps2_hardware_DmaSimulator_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// forwards
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DmaSimulator
{
public:
	DmaSimulator();
	~DmaSimulator();
	void start(u32* ptr);
protected:
	void disdma(u32* ptr);
	void disdata(u32 *ptr, int num);
	void disvif(u32 data, u32* ptr);
	void disgif(u32 data);
	void disad(int reg, u64 data);
	u32 readWord(u32 ptr);
	enum VIFSTATE { VS_READTAG, VS_DIRECT, VS_MPG, VS_UNPACK };
	VIFSTATE vifstate;
	enum GIFSTATE { GS_READTAG, GS_PACKED, GS_IMAGE };
	GIFSTATE gifstate;
	int gifqcount;
	int gifcount;
	//u128 giftag;
	//u128 gifqword;
	int nreg;
	int vifcount;
	int timeout;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}

}

#endif //zenic_ps2_hardware_DmaSimulator_h




