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
