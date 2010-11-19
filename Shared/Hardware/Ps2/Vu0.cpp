///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Vu0.h"
#include <Shared/Base/Io/File.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" u32 StandardMathVu0_CodeStart __attribute__((section(".vudata")));
extern "C" u32 StandardMathVu0_CodeEnd __attribute__((section(".vudata")));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

Vu0::User Vu0::m_user;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Vu0::setUser(User user, bool upload, bool waitFinised)
{
	Dma dmaChain((void*)0x70001000, 4096);

	switch (user)
	{
		case StandardMath : addToChain(dmaChain, &StandardMathVu0_CodeStart, &StandardMathVu0_CodeEnd); break; 
	}

	//dmaChain.addSrcEndTag();
	//dmaChain.sendChainVif0();

	m_user = user;

	//dmaChain.waitVif0();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Vu0::addToChain(Dma& dmaChain, u32* start, u32* end)
{
	s32 size = calcSize(start, end);

	/*

	ZENIC_ASSERT(size <= 256);

	uint offset = 0;

	while (size > 0)
	{
		u32 currentCount = size > 128 ? 128 : size;
		dmaChain.addSrcRefTag(start, currentCount >> 1, 0, VIF_MPG(currentCount, offset) );
		size -= currentCount;
		start += currentCount * 2;
		offset += 128;
	}


	*/

	// temp temp

	memcpy((void*)VU0_MICRO, &StandardMathVu0_CodeStart, size * 8);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Vu0::dumpOnce(const char* microMem,const char* mem)
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

	file.write((void*)VU0_MICRO, 1024*4);
	file.close();

	if (!file2.open(mem,File::Write))
		return;

	file2.write((void*)VU0_MEM, 1024*4);
	file2.close();

	hasDumped = true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 Vu0::calcSize(const u32* start,const u32* end)
{
	int size = (end-start)/2;
	
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


