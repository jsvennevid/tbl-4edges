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

#include "TextureSystem.h"

#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Hardware/Ps2/DmaSimulator.h>
#include <Shared/Hardware/Ps2/Vif.h>
#include <Shared/Hardware/Ps2/MasterChain.h>
#include <Shared/Base/Memory/New.h>
#include <Shared/Base/Memory/HeapAllocator.h>

#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Base/Memory/Prefetch.h>

#ifndef PS2_EMU
#include <kernel.h>
#endif
#include <string.h>

#define LINE ZENIC_INFO(__LINE__);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

namespace ps2
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TextureSystem::TextureSystem(Path path) : 
	m_currentTexture(0),
	m_gifSize(0), 
	m_topTexture(0), 
	m_base(MaxVideoMemory - (2*TextureMemSize)), 
	//m_base(0), 
	m_currentOffset(0),
	m_palOffset(0),
	m_buffer1(450 * 32),
	m_buffer2(450 * 32),
	m_path(path),
	m_flip(false)
{

	ZENIC_INFO("Base " << (m_base / 8192));

	if (m_path == Path2)
		return;

	// vif path 3 sync

	Dma p3sync(m_path3Sync, sizeof(m_path3Sync));

	// FLUSH & DIRECT(2) are inserted directly into the TTE
	p3sync.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
	p3sync.add128(0, GS_REG(TEXFLUSH));
	p3sync.add32(VIF_FLUSHA());
	p3sync.add32(VIF_MSKPATH3(0));

	for (uint i = 0; i < (NumNopQwords * 4); ++i)
		p3sync.add32(VIF_NOP());

	p3sync.add32(VIF_MSKPATH3(0x8000));
	p3sync.add32(VIF_NOP());

	// vif path 3 wait

	Dma p3wait(m_path3Wait, sizeof(m_path3Wait));

	// FLUSH & DIRECT(2) are inserted directly into the TTE
	p3wait.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
	p3wait.add128(0, GS_REG(TEXFLUSH));
	p3wait.add32(VIF_FLUSHA());
	p3wait.add32(VIF_NOP());
	p3wait.add32(VIF_NOP());
	p3wait.add32(VIF_NOP());

	// vif1 path3 mask

	Dma mask(m_path3Mask, sizeof(m_path3Mask));
	mask.addSrcEndTag(VIF_MSKPATH3(0x8000));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TextureSystem::~TextureSystem()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::create(uint size)
{
	if (m_path == Path2)
		return;

	m_size = size;

	HeapAllocator temp;

	m_gifBuffer1 = (u8*)temp.allocate(m_size);
	m_gifBuffer2 = (u8*)temp.allocate(m_size);

	flip();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::destroy()
{
	wait();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::setTexture(uint index, const Texture* texture)
{
	if (m_path == Path3)
		flush(texture);
	else
		flushPath2(texture);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::flush(const Texture* texture)
{
	const Texture* current = texture;

	ZENIC_ASSERT(current);

	if (!current)
		return;

	const Texture::Parameters& uploadParameters = current->uploadParameters();
	const Texture::Parameters& displayParameters = current->displayParameters();

	uint buffer = m_buffer1;
	DataPtr<u8> palette = current->palette();

	m_palOffset = buffer;

	if (palette)
	{
		uint x = 0;
		uint y = 0;

		if (displayParameters.psm() == gs::T4)
		{
			x = 16;
			y = 1;
		}
		else
		{
			x = 16;
			y = 16;
		}

		m_gifChain.addSrcCntTag();
		m_gifChain.add128(GIFTAG(4, 0, 0, 0, gif::Packed, 1), gif::A_D);
		m_gifChain.add128(GS_BITBLTBUF(0,0, 0, buffer, 1, 0), GS_REG(BITBLTBUF));
		m_gifChain.add128(GS_TRXPOS(0, 0, 0, 0, 0),GS_REG(TRXPOS));
		m_gifChain.add128(GS_TRXREG(x, y),GS_REG(TRXREG));
		m_gifChain.add128(GS_TRXDIR(0), GS_REG(TRXDIR));
		m_gifChain.add128(GIFTAG((x*y)/4, 0, 0, 0, gif::Image, 0), 0);

		m_gifChain.endPacket();
		m_gifChain.addSrcRefTag(palette.objects(), (x*y)/4);

		buffer += 32;
	}

	m_textureOffset = buffer;
	uint uploadWidth = uploadParameters.bufferWidth();

	for (uint j = 0, m = current->mipMaps(); j < m; ++j)
	{
		const DataPtr<u8>& data = current->data(j);
		m_gifChain.addSrcCntTag();
		m_gifChain.add128(GIFTAG(4, 0, 0, 0, gif::Packed, 1), gif::A_D);
		m_gifChain.add128(GS_BITBLTBUF(0, 0, 0, buffer, uploadParameters.bufferWidth(), uploadParameters.psm()), GS_REG(BITBLTBUF));
		m_gifChain.add128(GS_TRXPOS(0, 0, 0, 0, 0),GS_REG(TRXPOS));
		m_gifChain.add128(GS_TRXREG(uploadParameters.width(), uploadParameters.height()), GS_REG(TRXREG));
		m_gifChain.add128(GS_TRXDIR(0), GS_REG(TRXDIR));
		m_gifChain.add128(GIFTAG(data.count() >> 4, 0, 0, 0, gif::Image, 0), 0);

		m_gifChain.endPacket();
		m_gifChain.addSrcRefTag(data.objects(), data.count() >> 4);

		uploadWidth >>= 1;
		buffer = (buffer+data.count()+31) & ~31;
	}

	uint temp = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = temp;

	m_gifChain.addSrcCntTag(0);
	m_gifChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::RegList, 1), gif::A_D);
	m_gifChain.add128(0, GS_REG(NOP));
	m_gifChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::flushPath2(const Texture* texture)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	const Texture* current = texture;

	ZENIC_ASSERT(current);

	if (!current)
		return;

	const Texture::Parameters& uploadParameters = current->uploadParameters();
	const Texture::Parameters& displayParameters = current->displayParameters();

	uint buffer = m_buffer1;
	DataPtr<u8> palette = current->palette();

	m_palOffset = buffer;

	if (palette)
	{
		uint x = 0;
		uint y = 0;

		if (displayParameters.psm() == gs::T4)
		{
			x = 8;
			y = 2;
		}
		else
		{
			x = 16;
			y = 16;
		}

		dmaChain.addSrcCntTag(0, VIF_DIRECT(6));
		dmaChain.add128(GIFTAG(4, 0, 0, 0, gif::Packed, 1), gif::A_D);
		dmaChain.add128(GS_BITBLTBUF(0,0, 0, buffer, 1, 0), GS_REG(BITBLTBUF));
		dmaChain.add128(GS_TRXPOS(0, 0, 0, 0, 0),GS_REG(TRXPOS));
		dmaChain.add128(GS_TRXREG(x, y),GS_REG(TRXREG));
		dmaChain.add128(GS_TRXDIR(0), GS_REG(TRXDIR));
		dmaChain.add128(GIFTAG((x*y)/4, 0, 0, 0, gif::Image, 0), 0);

		dmaChain.endPacket();
		dmaChain.addSrcRefTag(palette.objects(), (x*y)/4, 0, VIF_DIRECT((x*y)/4));

		m_textures[m_currentTexture].palAddress = buffer;

		buffer += 32;
	}


	m_textures[m_currentTexture].vramAddress = buffer;
	m_textures[m_currentTexture].texture = texture;

	m_textureOffset = buffer;
	uint uploadWidth = uploadParameters.bufferWidth();

	for (uint j = 0, m = current->mipMaps(); j < m; ++j)
	{
		const DataPtr<u8>& data = current->data(j);
		dmaChain.addSrcCntTag(0, VIF_DIRECT(6));
		dmaChain.add128(GIFTAG(4, 0, 0, 0, gif::Packed, 1), gif::A_D);
		dmaChain.add128(GS_BITBLTBUF(0, 0, 0, buffer, uploadParameters.bufferWidth(), uploadParameters.psm()), GS_REG(BITBLTBUF));
		dmaChain.add128(GS_TRXPOS(0, 0, 0, 0, 0),GS_REG(TRXPOS));
		dmaChain.add128(GS_TRXREG(uploadParameters.width(), uploadParameters.height()), GS_REG(TRXREG));
		dmaChain.add128(GS_TRXDIR(0), GS_REG(TRXDIR));
		dmaChain.add128(GIFTAG(data.count() >> 4, 0, 0, 0, gif::Image, 0), 0);

		dmaChain.endPacket();
		dmaChain.addSrcRefTag(data.objects(), data.count() >> 4, 0, VIF_DIRECT(data.count() >> 4));

		uploadWidth >>= 1;
		buffer = (buffer+data.count()+31) & ~31;
	}

	m_currentTexture++;
	m_currentTexture %= MaxTextures;

	uint temp = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = temp;

	dmaChain.addSrcCntTag(0, VIF_DIRECT(2));
	dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::RegList, 1), gif::A_D);
	dmaChain.add128(0, GS_REG(NOP));
	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::begin()
{
	if (m_path == Path2)
		return;

	m_topTexture = 0;
	m_currentOffset = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::end()
{
	if (m_path == Path2)
		return;

	m_gifChain.addSrcEndTag(0);
	m_gifChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::sync()
{
	if (m_path == Path2)
		return;

	flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::addSync(Dma& dmaChain)
{
	if (m_path == Path2)
		return;

	dmaChain.addSrcCntTag(0);
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_FLUSH());
	dmaChain.add32(VIF_DIRECT(2));
	dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
	dmaChain.add128(0, GS_REG(TEXFLUSH));
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_FLUSHA());
	dmaChain.add32(VIF_MSKPATH3(0));

	for (uint i = 0; i < (NumNopQwords * 4); ++i)
		dmaChain.add32(VIF_NOP());

	dmaChain.add32(VIF_MSKPATH3(0x8000));
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());

	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::activateTexture(Dma& dmaChain, const Texture* texture, bool alpha)
{
	ZENIC_ASSERT(texture);
	ZENIC_PREFETCH_READ(texture);

	if (m_path == Path3)
		return;

	for (uint i = 0; i < MaxTextures; ++i)
	{
		if (m_textures[i].texture == texture)
		{
			const Texture::Parameters& displayParms = texture->displayParameters();

			dmaChain.addSrcCntTag(0, VIF_DIRECT(3));
			dmaChain.add128(GIFTAG(2, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
			dmaChain.add128(GS_TEXFLUSH(), GS_REG(TEXFLUSH));
			dmaChain.add128(GS_TEX0(m_textures[i].vramAddress, displayParms.bufferWidth(), displayParms.psm(), 
									displayParms.width(), displayParms.height(), alpha, 1, 
									m_textures[i].palAddress, 0, 0, 0, 1), GS_REG(TEX0_1));
			dmaChain.endPacket();

			break;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint TextureSystem::textureOffset()
{
	return m_base + m_currentOffset;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::flip()
{
	if (m_path == Path2)
		return;

	if (m_flip)
		m_gifChain.setData(m_gifBuffer1, m_size);
	else
		m_gifChain.setData(m_gifBuffer2, m_size);

	m_flip = !m_flip;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::kick()
{
	if (m_path == Path2)
		return;

	Dma::waitVif1();
	Dma::sendChainVif1(m_path3Mask);
	Dma::waitVif1();

	m_gifChain.sendChainGif();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureSystem::wait()
{
	if (m_path == Path2)
		return;

	Dma::waitGif();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

 }
