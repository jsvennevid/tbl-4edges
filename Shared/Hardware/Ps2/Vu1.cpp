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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Io/File.h>
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Hardware/Ps2/Vif.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Hardware/Ps2/MasterChain.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Vu1::clearCache()
{
	m_size = 0;
	m_programCount = 0;
	m_programAddress = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 Vu1::getProgram(Dma& dmaChain, const u32& id, u32* start, u32* end)
{
	ZENIC_ASSERT(start);
	ZENIC_ASSERT(end);

	for (uint i = 0; i < m_programCount; ++i)
	{
		if (m_programs[i].id == id)
			return m_programs[i].address;
	}

	uint size = calcSize(start, end);

	// if vu1 mem filled up, we start over.

	if ((size + m_size) > 1024)
	{
		m_size = 0;
		m_programAddress = 0;
		m_programCount = 0;
	}

	// insert new program

	m_programs[m_programCount].address = m_programAddress;
	m_programs[m_programCount].id = id;

	addToChain(dmaChain, start, end);

	m_programCount++;

	return m_programs[m_programCount-1].address;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 Vu1::getProgram(const u32& id)
{
	for (uint i = 0; i < m_programCount; ++i)
	{
		if (m_programs[i].id == id)
			return m_programs[i].address;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Vu1::addToChain(Dma& dmaChain, u32* start, u32* end)
{
	s32 size = calcSize(start, end);

	while (size > 0)
	{
		u32 currentCount = size > 128 ? 128 : size;
		dmaChain.addSrcRefTag(start, currentCount >> 1, 0, VIF_MPG(currentCount, m_programAddress));

		//Temporay fix for PC version
		//dmaChain.addSrcCntTag(0, VIF_MPG(currentCount, m_programAddress));
		//for (uint i = 0; i < currentCount << 1; ++i)
		//	dmaChain.add32(*start++);
		//dmaChain.align(16);

		size -= currentCount;
		start += currentCount * 2;
		m_programAddress += currentCount;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZENIC_PS2
void Vu1::dumpOnce(const char*,const char*) {}
#else
void Vu1::dumpOnce(const char* microMem, const char* mem)
{
	static bool hasDumped = false;

	if (hasDumped)
		return;

  asm __volatile__(
    "nop\n"
    "nop\n"
"0:\n"
    "bc2t 0b\n"
    "nop\n"
    "nop\n" );

	File file;
	File file2;

	if (!file.open(microMem,File::Write))
		return;

	file.write(((u8*)0x11008000),1024*16);
	file.close();

	if (!file2.open(mem,File::Write))
		return;

	file2.write(((u8*)0x1100c000),1024*16);
	file2.close();

	hasDumped = true;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 Vu1::calcSize(const u32* start,const u32* end)
{
	int size = ((int)end - (int)start)/2;
	
	// if size is odd we have make it even in order for the transfer to work
	// (quadwords, because of that its VERY important to have an extra nop nop
	// at the end of each vuprogram
	
	if (size & 1)
		size++;
	
	return size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

