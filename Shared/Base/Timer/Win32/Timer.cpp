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
#include "../../Debug/Assert.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Timer::reset()
{
	// setup high-resolution timer

	LARGE_INTEGER freq;
	LARGE_INTEGER base;

	BOOL success = QueryPerformanceFrequency(&freq);
	ZENIC_ASSERT_DESC(success, "Failed querying for timer frequency");
	success = false;
	QueryPerformanceFrequency(&base);

	m_frequency = freq.QuadPart;	
	m_base = base.QuadPart;

	// initialize timer

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
	LARGE_INTEGER value;

	QueryPerformanceCounter(&value);

	return u64(((value.QuadPart-m_base) * u64(Resolution)) / m_frequency);
//	return u64(((long double)(value.QuadPart-m_base)/(long double)(m_frequency)) * ((long double)Resolution));
}

}
