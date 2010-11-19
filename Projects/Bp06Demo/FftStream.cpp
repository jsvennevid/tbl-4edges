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

#include "FftStream.h"
#include <Shared/Base/Io/Log.h>
#if defined(ZENIC_PS2)
#include <fileXio_rpc.h>
#include <loadfile.h>
#endif
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

FftStream* FftStream::s_instance = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FftStream::FftStream() : 
	m_kickData(0), 
	m_hihatData(0),
	m_bassData(0),
	m_speachData(0),
	m_offset(0) 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FftStream::create()
{
	//m_kickData = loadData("DATA/OUTPUT/KICK.FFT");
	//m_hihatData = loadData("DATA/OUTPUT/HIHAT.FFT");
	m_bassData = loadData("DATA/OUTPUT/BASS.FFT");
	m_speachData = loadData("DATA/OUTPUT/RUNNING.FFT");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FftStream::setTime(float time)
{
	m_offset = uint((time * 60.0f) * 16.0f); // 60 fps and 16 samples per frame
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FftStream::update(Renderer& renderer, Dma& dmaChain, bool visualize)
{
	if (visualize)
	{
		ZENIC_ASSERT(m_speachData);

		PostEffectHelper hlp(renderer, dmaChain);
		hlp.setRgba(127, 127, 127, 127); 

		for (uint i = 0; i < 16; ++i)
		{
			hlp.blit(192, 200, 192 + 10 * i, 200 + (m_speachData[m_offset + i]), false, false);
			hlp.flushRegisters();
		}
	}


	m_offset += 16;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8* FftStream::loadData(const char* filename)
{
	FileSystem& fileSystem = FileSystem::instance();
	FileStream stream;

	if (!fileSystem.open(stream, filename))
		return false;

	int fileSize = 267952; // hax0r

	u8* data = zenic_new u8[fileSize];
	stream.read(data, fileSize);
	stream.close();

	return data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FftStream& FftStream::instance()
{
	if (!s_instance)
		s_instance = zenic_new FftStream;

	ZENIC_ASSERT(s_instance);
	return *s_instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

