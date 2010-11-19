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

#include "Timer.h"
#include <Shared/Base/Debug/Assert.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

u64 Timer::m_base = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Timer::reset()
{
	m_base = rawTick();
	m_deltaTick = 0;
	m_tick = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Timer::update()
{
	u64 newTick = rawTick();

	m_deltaTick = newTick - m_tick;
	m_tick = newTick;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u64 Timer::rawTick() const
{
	const float PS2_DIVIDER = 1000.f;
	u32 currTime;
	u64 lastTime = 0;
	u64 totalTime = 0;

	asm __volatile__ ("mfc0 %0, $9" : "=r" (currTime));
	u64 deltaTime = (currTime >= lastTime) ? (currTime - lastTime) : 0xffffffff - (lastTime - currTime);
	lastTime = currTime;

	deltaTime = (u64)((float)deltaTime / PS2_DIVIDER);
	totalTime += deltaTime;

	return (totalTime / 4) - m_base;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

