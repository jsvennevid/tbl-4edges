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

#include "TempTexture.h"
#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

#include <Shared/Base/Io/File.h>
#include <Shared/Base/Memory/New.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/DmaSimulator.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>

#ifdef ZENIC_WIN32
#pragma warning(disable : 4311)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(TempTexture, SERIALIZABLE_ID('P','S','2',' '), SERIALIZABLE_ID('T','M','P','T'));


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TempTexture::TempTexture() : m_imageOffset(0), m_palOffset(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TempTexture::~TempTexture()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TempTexture::serialize(Serializer& s)
{
	zenic::SerializableVersion version(1, factory());

	s << version;

	s.descriptor(SERIALIZER_NAME("m_callChain")) << m_callChain;
	s.descriptor(SERIALIZER_NAME("m_imageData")) << m_imageData;
	s.descriptor(SERIALIZER_NAME("m_palData")) << m_palData;

	s.descriptor(SERIALIZER_NAME("m_imageOffset")) << m_imageOffset;
	s.descriptor(SERIALIZER_NAME("m_palOffset")) << m_palOffset;

	// TODO: Only when texture is in use

	ZENIC_INFO("TempTexture::serialize");

	init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TempTexture::init()
{
	u8* callChainTemp = 0;
	u8* callChain = m_callChain.objects();
	
	callChainTemp = callChain + m_imageOffset;
	((u32*)callChainTemp)[1] = u32(m_imageData.objects());

	callChainTemp = callChain + m_palOffset;
	((u32*)callChainTemp)[1] = u32(m_palData.objects());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TempTexture::addToChain(Dma& dmaChain)
{
	dmaChain.addSrcCallTag(m_callChain.objects());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}

#ifdef ZENIC_WIN32
#pragma warning(default : 4311)
#endif

