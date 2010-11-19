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

#include "StartPart.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/TextureSystem.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>
#include <Shared/Input/Ps2/Pad.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static float s_syncPos[] =
{
	3.0f,		// fade time
	5.0,		// wait time
	6.0f,		// fade time

	8.0f,		// fade time
	10.0,		// wait time
	12.0f,		// fade time
};

extern Pad pad;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool StartPart::create()
{
	FileSystem& fileSystem = FileSystem::instance();

	FileStream stream;

	if (!fileSystem.open(stream, "DATA/OUTPUT/TBLLOGO.ZEI"))
		return false;

	InBinarySerializer inBinary;

	if (!inBinary.load(stream))
		return false;

	m_logo = static_cast<Texture*>(inBinary[0]);

	// PsLogo

	if (!fileSystem.open(stream, "DATA/OUTPUT/PSLOGO.ZEI"))
		return false;

	if (!inBinary.load(stream))
		return false;

	m_psLogo = static_cast<Texture*>(inBinary[0]);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StartPart::reset() 
{
	m_time = 0.0f;
	m_state = FadeUp;
	m_syncPos = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StartPart::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{

	if (time > s_syncPos[m_syncPos]) 
	{
		if (m_state == FadeUp) 
			m_state = Wait;
		else if(m_state == Wait)
			m_state = FadeDown;
		else
			m_state = FadeUp;

		m_time = 0.0f;
		m_syncPos++;
	}

	switch (m_state) 
	{
		case FadeUp :
		{
			showImage(m_time);
			break;
		}

		case Wait :
		{
			showImage(1.0f);
			break;
		}

		case FadeDown :
		{
			m_time += deltaTime;
			showImage(1.0f - m_time);
			break;
		}
	}

	m_time += deltaTime * 0.5f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StartPart::showImage(float fade)
{
	if (fade > 1.0)
		fade = 1.0f;

	if (fade < 0.0)
		fade = 0.0f;

	TextureSystem& textureSystem = m_renderer.textureSystem();
	Texture* currentTexture = 0;

	float gainStrength = 1.19;

	switch (m_syncPos) 
	{
		case 0 :
		case 1 :
		case 2 : currentTexture = m_logo; break;

		case 3 :
		case 4 :
		case 5 : 
		{
			gainStrength = 1.19;
			currentTexture = m_psLogo;
			break;
		}
	}

	if (!currentTexture)
		return;

	textureSystem.setTexture(0, currentTexture);
	textureSystem.sync();
	textureSystem.activateTexture(m_dmaChain, currentTexture);

	// ugly hack for white fade
/*
	if (m_state == FadeDown && m_syncPos == 5)
	{
		fade = 1.0f + (1.0f - fade);
		if (fade > 1.99f)
			fade = 1.99f;

		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.setRgba(u32(fade * 127.0f), u32(fade * 127.0f), u32(fade * 127.0f), u32(fade * 127.0f)); 
		hlp.setTex0Pal((450*32) + 32, (450*32), 8, 20, 9, 9, 0, 2);
		hlp.blit(0, 0, 512, 448, 0.0f, 0.0f, 512.0f, 448.0f, false, true);
		//hlp.glare(0.0, 2, fade, 1.59f - (fade * 0.15f));
	}
	else
	{
*/
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.setRgba(u32(fade * 127.0f), u32(fade * 127.0f), u32(fade * 127.0f), u32(fade * 50.0f)); 
		hlp.setTex0Pal((450*32) + 32, (450*32), 8, 20, 9, 9);
		hlp.blit(0, 0, 512, 448, 0.0f, 0.0f, 512.0f, 448.0f, false, true);

		//hlp.glare(0.0, 1, 1.0f, 1.0f + (1.0f - fade) * 2.0f);
//	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
