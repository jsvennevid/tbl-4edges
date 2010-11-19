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

#ifndef zenic_ps2_ShaderSystem_h
#define zenic_ps2_ShaderSystem_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Dma;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ShaderSystem
{
public:

	enum
	{
		MaxSimultaneousShaders = 8
	};

	struct Shader
	{
		u32 address;
		u32* start;
		u32* end;
		bool inplace;
		bool uploaded;
	};

	ShaderSystem(uint endAddress = 1023);

	void activate();
	void deactivate();

	void activateShaders(Shader* shaders, uint shaderCount);

private:

	void reset();
	uint shaderListSize(Shader* shaders, uint shaderCount);
	void uploadShaders(Dma& dmaChain, Shader* shaders, uint shaderCount);
	void uploadShader(Dma& dmaChain, Shader& shader);
	static uint calcSize(const u32* start, const u32* end);

	enum
	{
		MaxShaders = 32	// Max amount of shaders at once.
	};

	Shader m_shaders[MaxShaders];

	u32 m_shellStart;
	u32 m_shaderCount;
	u32	m_startAddress;
	u32 m_currentAddress;
	u32 m_endAddress;
	bool m_active;

	// opcodes used when building jumptable

	static const u32 m_opcodeNopUpper;
	static const u32 m_opcodeNopLower;
	static const u32 m_opcodeBalRetAddress;
	static const u32 m_opcodeJrVi15;

	static const u32 m_opcodesSwapBuffer[4];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
