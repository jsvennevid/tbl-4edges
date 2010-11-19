///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_ps2_Vu1_h
#define zenic_ps2_Vu1_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Base/Storage/Singleton.h>
#include <Shared/Base/Storage/Array.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Dma;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Vu1 : public Singleton<Vu1>
{
public:

	enum
	{
		MaxPrograms = 32
	};

	// Ids for vu programs

	enum
	{
		BranchRenderer = 2,

		IntFlatNonShaded,
		IntTexturedNonShaded,

		FloatFlatNonShaded,
		FloatTexturedNonShaded,

		AlphaDissolveEnvmapped,
		ShRenderer,
		Sprite,

		CubeRenderer
	};


	s32 getProgram(Dma& dmaChain, const u32& id, u32* start, u32* end);
	s32 getProgram(const u32& id);
	void addToChain(Dma& dmaChain, u32* start, u32* end);
	void clearCache();
	static void dumpOnce(const char* microMem = "host:vu1micromem", const char* mem = "host:vu1mem");
	u32 currentLocation() const;

private:

	static u32 calcSize(const u32* start, const u32* end);

	struct Program
	{
		u32 id;
		u32 address;
	};

	Program m_programs[MaxPrograms];
	u32 m_size;
	u32 m_programCount;
	s32 m_programAddress;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 Vu1::currentLocation() const
{
	return (u32)m_programAddress;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif //zenic_ps2_Vu1_h
