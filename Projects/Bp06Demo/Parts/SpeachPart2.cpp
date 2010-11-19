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

#include "SpeachPart2.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Geometry/Geometry.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/CubeRenderer.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>
#include "../PadCamera.h"
#include "../FftStream.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

static float s_startTime = 195.40f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SpeachPart2::create()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SpeachPart2::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	m_renderer.setClearColor(0, 0, 0);

	GS_WRITE(BGCOLOR,GS_BGCOLOR(255,0,0));

	Model::resetCollectedModelCount();
	m_scene->rootNode()->update(time);
	m_renderer.setSceneCamera(m_scene->primaryCamera());
	m_renderer.renderScene(m_scene);
	m_renderer.camera().setViewWindowSize(1.1f);

	Matrix4 mscale;

	m_scene->switchToCamera(2);

	mscale = Matrix4::identity();

	FftStream& stream = FftStream::instance();

	addDisolveObject(stream.speachData(), 2, mscale);

	if (time < 2.0f)
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeDownFrame(0, 0, 0, time * 0.5f);
	}


	if (time > (206.00f - s_startTime) && time < (207.00f - s_startTime))
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeFrame(255, 255, 255, ((207.00f - s_startTime) - time));
	}

	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.setRgba(255, 255, 255, 128);
		hlp.setAlpha(0, 1, 2, 2, 128);
		hlp.blit(0, 0, 512, 448, true, false);
	}

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.motionBlur(0x30);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}





