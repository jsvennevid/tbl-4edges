///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

