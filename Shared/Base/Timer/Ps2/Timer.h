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

#ifndef zenic_ps2_Timer_h
#define zenic_ps2_Timer_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../Types.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Timer
{
public:

	enum { Resolution = 100000 };

	Timer();

	void reset();
	void update();

	u64 deltaTick() const;
	u64 tick() const;

	static f32 toSeconds(u64 tick);

private:

	u64 rawTick() const;

	u64 m_deltaTick;
	u64 m_tick;

	static u64 m_base;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Timer.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif

