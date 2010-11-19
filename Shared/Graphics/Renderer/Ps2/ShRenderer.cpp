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

#include "ShRenderer.h"
#include <Shared/Base/Math/Vector3.h>
#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>

#define M_PI 3.1415926535897932384626433832795f

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PS2_EMU
extern "C" u32 ShRendererVu1_CodeStart __attribute__((section(".vudata")));
extern "C" u32 ShRendererVu1_CodeEnd __attribute__((section(".vudata")));
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ShRenderer::ShRenderer(Renderer& renderer, Dma& dmaChain) : 
	m_renderer(renderer),
	m_dmaChain(dmaChain),
	m_startAddress(0)
{
	// Leet test texture!

	Texture::Parameters uploadParameters(4, gs::T8, 0, 256, 256);
	Texture::Parameters displayParameters(4, gs::T8, 0, 8, 8);

	DataPtr<u8> pal;
	DataPtr<u8> texture;

	pal.allocate(256 * 4);
	u32* palObjects = (u32*)pal.objects();

	texture.allocate(256 * 256);
	u8* textureData = texture.objects();

	for (u32 p = 0;  p < 256; ++p)
		palObjects[(p & 231) + ((p & 8) << 1) + ((p & 16) >> 1)] = (p << 24) | (p << 16) | (p << 8) | p;

	for (uint y = 0; y < 256; ++y)
		for (uint x = 0; x < 256; ++x)
			textureData[(y << 8) + x] = x^y;

	m_tempTexture.setUploadParameters(uploadParameters);
	m_tempTexture.setDisplayParameters(displayParameters);
	m_tempTexture.setMipMaps(1);
	m_tempTexture.setPalette(pal);
	m_tempTexture.setData(0, texture);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShRenderer::begin()
{
	m_renderer.beginRenderModels();

	m_dmaChain.addSrcCntTag(0, VIF_FLUSH());
	m_dmaChain.endPacket();

#ifndef PS2_EMU
	m_startAddress = Vu1::instance()->getProgram(m_dmaChain, Vu1::ShRenderer, 
												 &ShRendererVu1_CodeStart, 
												 &ShRendererVu1_CodeEnd);
#endif
	TextureSystem& textureSystem = m_renderer.textureSystem();

	textureSystem.addSync(m_dmaChain);
	textureSystem.setTexture(0, &m_tempTexture);
	textureSystem.sync();
	textureSystem.activateTexture(m_dmaChain, &m_tempTexture, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShRenderer::render(const Parameters& parameters, const u8* fftData, const Matrix4& matrix)
{
	m_dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 8, 10));

	const float* matrixData = matrix.matrix();

	for (uint j = 0; j < 16 ; ++j)
		m_dmaChain.addFloat(*matrixData++);

	// animatable parameters

	static float counter1 = 0;
	static float counter2 = 0;
	static float counter3 = 0;
	static float counter4 = 0;

	float increment[4];
	for (uint i = 0; i < 4; ++i)
	{
		float temp = 0;
		for (uint j = 0; j < 4; ++j)
			temp += (float(fftData[i * 4 + j]) / 255.0f) * 0.25f;

		increment[i] = temp * 0.5f;
	}

	counter1 += increment[0];
	counter2 += increment[1];
	counter3 += increment[2];
	counter4 += increment[3];

	m_dmaChain.addFloat(2 + cosf(counter1)); // parameter 0
	m_dmaChain.addFloat(0); // pad
	m_dmaChain.addFloat(2 + cosf(counter2)); // parameter 2
	m_dmaChain.addFloat(0); // pad

	m_dmaChain.addFloat(2 + cosf(counter3)); // parameter 4
	m_dmaChain.addFloat(0); // pad
	m_dmaChain.addFloat(2 + cosf(counter4)); // parameter 6
	m_dmaChain.addFloat(0); // pad

	m_dmaChain.addFloat(2); // parameter 1
	m_dmaChain.addFloat(2); // parameter 3
	m_dmaChain.addFloat(2); // parameter 5
	m_dmaChain.addFloat(2); // parameter 7

	m_dmaChain.addFloat((M_PI * 2) / (64+1)); // theta
	m_dmaChain.addFloat(0); // pad
	m_dmaChain.addFloat((M_PI) / 128); // phi
	m_dmaChain.addFloat(0); // pad

	m_dmaChain.endPacket();

	// Lets start the stuff!

	m_dmaChain.addSrcCntTag(0);
	m_dmaChain.add32(VIF_NOP());
	m_dmaChain.add32(VIF_NOP());
	m_dmaChain.add32(VIF_NOP());
	m_dmaChain.add32(VIF_MSCAL(m_startAddress));
	m_dmaChain.endPacket();

	static int p = 0;

	if (p == 4)
	{
		//Vu1::dumpOnce();
	}

	++p;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShRenderer::end()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}


