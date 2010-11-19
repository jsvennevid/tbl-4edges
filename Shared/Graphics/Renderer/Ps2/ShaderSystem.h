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
