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

#ifndef zenic_ps2_ParkingHousePart_h
#define zenic_ps2_ParkingHousePart_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Part.h"
#include <Shared/Base/Types.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/FadeToFog.h>
#include <Shared/Graphics/Renderer/Ps2/RenderSettings.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/DepthOfField.h>
#include <Shared/Graphics/Renderer/Ps2/RenderSettings.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class SceneInfo;

	namespace ps2
	{
		class Texture;
		class Dma;
		class Renderer;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParkingHousePart : public Part
{

public:

	ParkingHousePart(Renderer& renderer, Dma& dmaChain);

	bool create();
	void update(float time, float deltaTime, SyncPoints::Part* syncPoints = 0);
	void reset() {};

private:

	enum FadeState
	{
		FlashShow,
		Show,
		FlashHide
	};

	void showImage4(const Texture* texture, float strength);
	Texture* loadTexture(const char* filename);

	FadeToFog m_fadeToFogPostEffect;
	Renderer& m_renderer;
	Dma& m_dmaChain;
	SceneInfo* m_scene;
	Texture* m_textures[9];
	FadeState m_fadeState;
	uint m_currentGreeting;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ParkingHousePart::ParkingHousePart(Renderer& renderer, Dma& dmaChain) :
	m_fadeToFogPostEffect(RenderSettings()),
	m_renderer(renderer),
	m_dmaChain(dmaChain),
	m_scene(0),
	m_fadeState(ParkingHousePart::FlashShow),
	m_currentGreeting(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif




