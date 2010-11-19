///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
