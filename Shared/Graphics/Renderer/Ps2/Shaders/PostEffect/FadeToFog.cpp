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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FadeToFog.h"
#include "PostEffectBuilder.h"
#include "PostEffectHelper.h"

#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Base/Io/Log.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FadeToFog::FadeToFog(const RenderSettings& renderSettings) :
	m_settings(renderSettings)
{
	u32 tempPal[256];

	// TODO: Check if we can reduce the size of these allocs

	m_tessel1 = zenic_new u8[16 * 1024];
	m_tessel2 = zenic_new u8[16 * 1024];
	m_pal = zenic_new u8[2 * 1024];

	for (uint i = 0; i < 256; ++i)
		tempPal[i] = ((i / 2)<<24) | 0x808080;

	PostEffectBuilder::buildPaletteUploadChain(m_pal, 510, tempPal);


	PostEffectBuilder::buildIndexChain(m_tessel2, m_settings.zbuffer(), m_settings.buffer0(), 
												  m_settings.zbuffer(), 510, 0, 0, 
												  m_settings.screenWidth(), m_settings.screenHeight());

	PostEffectBuilder::buildIndexChain(m_tessel1, m_settings.zbuffer(), m_settings.buffer1(), 
												  m_settings.zbuffer(), 510, 0, 0, 
												  m_settings.screenWidth(), m_settings.screenHeight());


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FadeToFog::~FadeToFog()
{
	zenic_delete [] m_tessel1;
	zenic_delete [] m_tessel2;
	zenic_delete [] m_pal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FadeToFog::render(uint r, uint g, uint b, uint currentScreen, Dma& dmaChain)
{
	dmaChain.addSrcCntTag();
	dmaChain.add32(VIF_FLUSH());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_DIRECT(4));

	dmaChain.add128(GIFTAG(3, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
	dmaChain.add128(GS_TEST(0, 0, 0, 0, 0, 0, 1, 2), GS_REG(TEST_2));
	dmaChain.add128(GS_ALPHA(1, 0, 0, 0, 0), GS_REG(ALPHA_2));
	dmaChain.add128(GS_RGBAQ(r, g, b, 0x80, 1.0),  GS_REG(RGBAQ));
	dmaChain.endPacket();

	PostEffectBuilder::uploadIndexChain(dmaChain, currentScreen ? m_tessel1 : m_tessel2, 1, 0, true, 0x00ffff, true, m_pal);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
