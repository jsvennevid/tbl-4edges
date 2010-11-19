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

#include "LoadingPart.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/TextureSystem.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool LoadingPart::create()
{
	FileSystem& fileSystem = FileSystem::instance();

	FileStream stream;

	if (!fileSystem.open(stream, "DATA/OUTPUT/LOADING.ZEI"))
		return false;

	InBinarySerializer inBinary;

	if (!inBinary.load(stream))
		return false;

	m_loading = static_cast<Texture*>(inBinary[0]);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadingPart::update(float progress, float time, SyncPoints::Part* syncPoints)
{
	if (time > 0.0f)
	{
		showImage();
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeDownFrame(0, 0, 0, 0.99f - time);
	}
	else
	{
		showImage();
		m_renderer.flip();
		showImage();
		m_renderer.flip();
		showImage();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadingPart::showImage()
{
	ZENIC_ASSERT(m_loading);

	TextureSystem& textureSystem = m_renderer.textureSystem();
	textureSystem.setTexture(0, m_loading);
	textureSystem.sync();
	textureSystem.activateTexture(m_dmaChain, m_loading);

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.setRgba(0, 0, 0, 0); 
	hlp.setTex0Pal((450*32) + 32, (450*32), 2, 20, 7, 7, 0, 1);
	hlp.blit(0, 0, 512, 448);
	hlp.blit(192, 200, 192 + 128, 200 + 64, 0.0f, 0.0f, 128.0f, 64.0f, false, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

