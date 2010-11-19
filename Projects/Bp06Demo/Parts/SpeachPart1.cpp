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

#include "SpeachPart1.h"
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

static const float s_startTime = 55.83;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SpeachPart1::create()
{
	loadScene("DATA/OUTPUT/SPEACH.ZES");
	m_texts[0] = loadTexture("DATA/OUTPUT/STEXT0.ZEI");
	m_texts[1] = loadTexture("DATA/OUTPUT/STEXT1.ZEI");
	m_texts[2] = loadTexture("DATA/OUTPUT/STEXT2.ZEI");
	m_texts[3] = loadTexture("DATA/OUTPUT/STEXT3.ZEI");
	m_texts[4] = loadTexture("DATA/OUTPUT/STEXT4.ZEI");

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SpeachPart1::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	m_renderer.setClearColor(0, 0, 0);

	GS_WRITE(BGCOLOR,GS_BGCOLOR(255,0,0));

	m_scene->switchToCamera(1);

	Model::resetCollectedModelCount();
	m_scene->rootNode()->update(time);
	m_renderer.setSceneCamera(m_scene->primaryCamera());
	m_renderer.renderScene(m_scene);
	m_renderer.camera().setViewWindowSize(1.1f);
	m_renderer.setDisableOrgGlowObjects(true);

	// Blur glow parts

	{
		//PostEffectHelper hlp(m_renderer, m_dmaChain);
		//hlp.glareOffScreen(336, 0.4f, 1, 1.0f, 1.6f);
	}

	if (time > (61.39f - s_startTime) && time < (62.39f - s_startTime))
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeFrame(255, 255, 255, ((62.39f - s_startTime) - time) * 0.5f);
	}

	if (time > (66.95f - s_startTime) && time < (67.95f - s_startTime))
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeFrame(255, 255, 255, ((67.95f - s_startTime) - time) * 0.5f);
	}


	//Matrix4 matrix;

	//matrix = Matrix4::identity();
	//m_renderer.renderAdditveModel(m_models[0], matrix);


	Matrix4 mscale;

	mscale = Matrix4::identity();

	FftStream& stream = FftStream::instance();

	addDisolveObject(stream.speachData(), 2, mscale);

	//if (!m_padCamera)
	//else
	//m_renderer.setSceneCamera(&m_padCamera->sceneCamera());
	m_renderer.setSceneCamera(m_scene->primaryCamera());

	if (time < 2.0f)
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeDownFrame(0, 0, 0, time * 0.5f);
	}

	if (time > (70.89f - s_startTime) && time < (71.09f - s_startTime))
		showImage(m_texts[0]);

	if (time > (71.09f - s_startTime) && time < (71.26f - s_startTime))
		showImage(m_texts[1]);

	if (time > (71.26f - s_startTime) && time < (71.38f - s_startTime))
		showImage(m_texts[2]);

	if (time > (71.38f - s_startTime) && time < (71.65f - s_startTime))
		showImage(m_texts[3]);

	if (time > (71.65f - s_startTime))
		showImage(m_texts[4]);


	m_fade -= deltaTime * 2.0f;

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.motionBlur(0x30);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SpeachPart1::showImage(const Texture* texture, float strength)
{
	ZENIC_ASSERT(texture);

	TextureSystem& textureSystem = m_renderer.textureSystem();
	textureSystem.setTexture(0, texture);
	textureSystem.sync();
	textureSystem.activateTexture(m_dmaChain, texture);

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.setRgba(0, 0, 0, 0); 
	hlp.setTex0Pal((450*32) + 32, (450*32), 8, 20, 9, 9, 0, 1);
	hlp.setAlpha(0, 2, 2, 1, (u32)(strength * 128.0f));
	hlp.blit(0, 0, 512, 448, 0.0f, 0.0f, 512.0f, 448.0f, true, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture* SpeachPart1::loadTexture(const char* filename)
{
	FileSystem& fileSystem = FileSystem::instance();
	InBinarySerializer inBinary;

	FileStream stream;

	if (!fileSystem.open(stream, filename))
		return false;

	if (!inBinary.load(stream))
		return false;

	return static_cast<Texture*>(inBinary[0]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}




