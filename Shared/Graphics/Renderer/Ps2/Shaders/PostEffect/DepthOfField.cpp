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

#include "DepthOfField.h"
#include "PostEffectBuilder.h"
#include "PostEffectHelper.h"

#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Base/Io/Log.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DepthOfField::DepthOfField(const RenderSettings& renderSettings) :
	m_settings(renderSettings)
{
	u32 tempPal[256];

	// TODO: Chechk if we can reduce the size of these allocs

	m_tessel1 = zenic_new u8[16 * 1024];
	m_tessel2 = zenic_new u8[16 * 1024];
	m_pal = zenic_new u8[2 * 1024];

	for (uint i = 0; i < 256; ++i)
	{
		float n = (float)i / 256;
		tempPal[i] = (int)(n*n*n * 128) * 0x1010101;
	}

	PostEffectBuilder::buildPaletteUploadChain(m_pal, 510, tempPal);

	PostEffectBuilder::buildIndexChain(m_tessel2, m_settings.zbuffer(), m_settings.buffer0(), 
												  m_settings.zbuffer(), 510, 0, 0, 
												  m_settings.screenWidth(), m_settings.screenHeight());

	PostEffectBuilder::buildIndexChain(m_tessel1, m_settings.zbuffer(), m_settings.buffer1(), 
												  m_settings.zbuffer(), 510, 0, 0, 
												  m_settings.screenWidth(), m_settings.screenHeight());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DepthOfField::~DepthOfField()
{
	zenic_delete [] m_tessel1;
	zenic_delete [] m_tessel2;
	zenic_delete [] m_pal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DepthOfField::render(uint currentScreen, Renderer& renderer, Dma& dmaChain)
{

	{
		// clear alpha channel to 0x80
		PostEffectHelper hlp(renderer, dmaChain);
		hlp.setFrame(currentScreen, m_settings.screenFbw(), 0, 0x00ffffff);
		hlp.setRgba(0, 0, 0, 0x80);
		hlp.blit(0, 0, m_settings.screenWidth(), m_settings.screenHeight());
	}

	dmaChain.addSrcCntTag();
	dmaChain.add32(VIF_FLUSH());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());

	dmaChain.add32(VIF_DIRECT(3));
	dmaChain.add128(GIFTAG(2, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_TEST(0, 0, 0, 0, 0, 0, 1, 2), GS_REG(TEST_2));
	dmaChain.add128(GS_RGBAQ(0x80, 0x80, 0x80, 0x80, 1.0),  GS_REG(RGBAQ));
	dmaChain.endPacket();

	PostEffectBuilder::uploadIndexChain(dmaChain, currentScreen ? m_tessel1 : m_tessel2, 1, 0x00ffffff, true,
										0x00ffff, false, m_pal);

	{
		PostEffectHelper hlp(renderer, dmaChain);

		hlp.setFrame(224, 8);
		hlp.blit(0, 0, 512, 448, false, true);

		hlp.blur(224, 336, 8, 512, 448, 4, 1.0f, 0);
		hlp.setClamp(2, 2, 0, 511, 0, 448);

		hlp.setClamp(2, 2, 0, 511, 0, 447);
		hlp.setFrame();
		hlp.setTex0(224, 8);
		hlp.setAlpha(1, 0, 1, 0, 0);
		hlp.blit(0, 0, 512, 448, -1.0, -1.0f, 511.0f, 447.0f, true, true);

/*
		PostEffectHelper hlp(renderer, dmaChain);

		hlp.setFrame(m_settings.zbuffer(), 8);
		hlp.blit(0, 0, m_settings.screenWidth(), m_settings.screenHeight(), false, true);

		hlp.blur(m_settings.zbuffer(), m_settings.tempBuffer0(), 
				 8, m_settings.screenWidth(), m_settings.screenHeight(), 4, 1.0f, 0);

		hlp.setClamp(2, 2, 0, m_settings.screenWidth() - 1, 0, m_settings.screenHeight() - 1);
		hlp.setFrame();
		hlp.setTex0(m_settings.zbuffer(), m_settings.textureWidth());
		hlp.setAlpha(1, 0, 1, 0, 0);
		hlp.blit(0, 0, m_settings.screenWidth(), m_settings.screenHeight(), 
				 -1.0, -1.0f, m_settings.screenWidth() - 1.0f, m_settings.screenHeight() - 1.0f, true, true);
*/
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
