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

#include <Shared/Hardware/Ps2/MasterChain.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Base/Io/Log.h>
#include "ShaderSystem.h"
#ifndef PS2_EMU 
#include <kernel.h>
#endif
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PS2_EMU
extern "C" u32 ShaderShellVu1_CodeStart __attribute__((section(".vudata")));
extern "C" u32 ShaderShellVu1_CodeEnd __attribute__((section(".vudata")));

extern "C" u32 ShaderShellStub_NextChunk __attribute__((section(".vudata")));
extern "C" u32 ShaderShellStub_End __attribute__((section(".vudata")));

extern "C" u32 ShaderSystemStub_CodeStart __attribute__((section(".vudata")));
extern "C" u32 ShaderSystemStub_CodeEnd __attribute__((section(".vudata")));

#else

u32 ShaderShellVu1_CodeStart;
u32 ShaderShellVu1_CodeEnd;

u32 ShaderShellStub_NextChunk;
u32 ShaderShellStub_End;

u32 ShaderSystemStub_CodeStart;
u32 ShaderSystemStub_CodeEnd;

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const u32 ShaderSystem::m_opcodeNopLower = 0x8000033c;  // nop
const u32 ShaderSystem::m_opcodeNopUpper = 0x000002ff; // nop
const u32 ShaderSystem::m_opcodeBalRetAddress = 0x42000000 | 15 << 16; // bal vi15, x
const u32 ShaderSystem::m_opcodeJrVi15 = 0x48007800; // jr vi15

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ShaderSystem::ShaderSystem(uint endAddress) : 
	m_shaderCount(0), 
	m_startAddress(0), 
	m_endAddress(endAddress),
	m_active(false)
{
	memset(m_shaders, 0, sizeof(Shader) * MaxShaders);

	m_shellStart = calcSize(&ShaderShellVu1_CodeStart, &ShaderShellStub_NextChunk) + 1;

#ifndef ZENIC_FINAL
	uint shellSize = calcSize(&ShaderShellStub_NextChunk, &ShaderShellStub_End);
#endif
	uint stubSize = calcSize(&ShaderSystemStub_CodeStart, &ShaderSystemStub_CodeEnd);

	// prepatch the shader shell.

	ZENIC_ASSERT_DESC(shellSize == stubSize, "ShaderStub and Shell doesnt match.");

	memcpy(&ShaderShellStub_NextChunk, &ShaderSystemStub_CodeStart, (stubSize-1) * 8);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShaderSystem::activate()
{
	Vu1* vu1Handler = Vu1::instance();

	vu1Handler->clearCache();
	vu1Handler->getProgram(MasterChain::instance()->chain(), 3, &ShaderShellVu1_CodeStart, &ShaderShellVu1_CodeEnd);

	m_startAddress = m_currentAddress = vu1Handler->currentLocation();
	m_active = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShaderSystem::deactivate()
{
	m_active = false;

	memset(m_shaders, 0, sizeof(Shader) * MaxShaders);
	m_shaderCount = 0;
	m_startAddress = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShaderSystem::activateShaders(Shader* shaders, uint shaderCount)
{
	ZENIC_ASSERT_DESC(shaderCount < MaxSimultaneousShaders, "Maximum simultaneous shaders exceeded");

	uint foundShaderCount = 0;
	Dma& dmaChain = MasterChain::instance()->chain();

	// check if all shaders is in memory already

	for (uint i = 0; i < m_shaderCount; ++i)
	{
		for (uint p = 0; p < shaderCount; ++p)
		{
			if (m_shaders[i].start == shaders[p].start)
			{
				foundShaderCount++;
				shaders[p].address = m_shaders[i].address; 
				shaders[p].uploaded = true;
				break;
			}
		}
	}

	// if all shaders arent uploaded, we need to add them to the chain
	// and "shaders" struct will be filled with correct address here

	if (foundShaderCount != shaderCount)
	{
		uploadShaders(dmaChain, shaders, shaderCount);
	}

	// We may be able to do one optimization here, if the shaders is located on the same addresses as the last
	// time we actually dont need to add this mpg here.

	dmaChain.addSrcCntTag(0, VIF_MPG(MaxSimultaneousShaders << 1, m_shellStart));

	// we test just adding noops for now

	uint programCounter = m_shellStart + 2;

	for (uint i = 0; i < shaderCount; ++i)
	{
		dmaChain.add32(m_opcodeBalRetAddress | shaders[i].address - programCounter); // jump is pc relative
		dmaChain.add32(m_opcodeNopUpper);

		dmaChain.add32(m_opcodeNopLower);
		dmaChain.add32(m_opcodeNopUpper);

		programCounter += 2;
	}

	// dummy write nop's to the slots not used

	for (uint i = 0; i < MaxSimultaneousShaders - shaderCount; ++i)
	{
		dmaChain.add32(m_opcodeNopLower);
		dmaChain.add32(m_opcodeNopUpper);

		dmaChain.add32(m_opcodeNopLower);
		dmaChain.add32(m_opcodeNopUpper);
	}

	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShaderSystem::uploadShaders(Dma& dmaChain, Shader* shaders, uint shaderCount)
{
	uint totalShaderSize = shaderListSize(shaders, shaderCount);

	// make sure that we havet wrapped the memory

	if ((m_currentAddress + totalShaderSize) > m_endAddress)
	{
		reset();

		for (uint i = 0; i < shaderCount; ++i)
			shaders[i].uploaded = false;
	}

	// check so that the shaders actually fit the memory

#ifndef ZENIC_FINAL
	totalShaderSize = shaderListSize(shaders, shaderCount);
	ZENIC_ASSERT_DESC((m_currentAddress + totalShaderSize) < m_endAddress, "Amount of shaders dosent fit codememory");	
#endif

	dmaChain.addSrcCntTag(VIF_FLUSH());
	dmaChain.endPacket();

	for (uint i = 0; i < shaderCount; ++i)
	{
		//if (!shaders[i].uploaded)
		{
			uploadShader(dmaChain, shaders[i]);
		}

		ZENIC_ASSERT_DESC(m_shaderCount < MaxShaders, "Maximum ShaderLimit crossed");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShaderSystem::uploadShader(Dma& dmaChain, Shader& shader)
{
	s32 size = calcSize(shader.start, shader.end);
	u32* start = shader.start;

	shader.uploaded = true;
	shader.address = m_currentAddress;

	while (size > 0)
	{
		u32 currentCount = size > 128 ? 128 : size;
		dmaChain.addSrcRefTag(start, currentCount >> 1, 0, VIF_MPG(currentCount, m_currentAddress));
		size -= currentCount;
		start += currentCount * 2;
		m_currentAddress += currentCount;
	}


	// TODO: Remove this code if possible
	// We need this for adding a jr vi15 at the end of the shader as vcl doesnt like to do it for us.

	dmaChain.addSrcCntTag(0,VIF_MPG(1 << 1, m_currentAddress - 2));

	dmaChain.add32(m_opcodeJrVi15);
	dmaChain.add32(m_opcodeNopUpper);

	dmaChain.add32(m_opcodeNopLower);
	dmaChain.add32(m_opcodeNopUpper);

	dmaChain.endPacket();

	m_shaders[m_shaderCount].address = shader.address;
	m_shaders[m_shaderCount].start = shader.start;
	m_shaders[m_shaderCount].end = shader.end;
	m_shaders[m_shaderCount].uploaded = true;
	m_shaderCount++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint ShaderSystem::shaderListSize(Shader* shaders, uint shaderCount)
{
	uint totalShaderSize = 0;

	for (uint i = 0; i < shaderCount; ++i)
	{
		if (!shaders[i].uploaded)
			totalShaderSize += calcSize(shaders[i].start, shaders[i].end);
	}

	return totalShaderSize;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShaderSystem::reset()
{
	m_shaderCount = 0;
	m_currentAddress = m_startAddress;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 ShaderSystem::calcSize(const u32* start, const u32* end)
{
	uint size = (end-start)/2;

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

