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

#include "SaPart1.h"
#include "SaPart.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Graphics/Scene/Camera.h>
#include <Shared/Geometry/Geometry.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/CubeRenderer.h>
#include <Shared/Graphics/Renderer/Ps2/ShRenderer.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>
#include "../FftStream.h"
#include "../PadCamera.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SaPart1::create()
{
	loadScene("DATA/OUTPUT/SASCENE1.ZES");
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart1::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	static float testTime = 0.0f;
	static float cameraTime = ((60.0f / 172.0f) * 1.0f);
	static int camera = 1;

	ZENIC_ASSERT(m_scene);
	m_renderer.setClearColor(40, 40, 40);
	m_renderer.setDisableOrgGlowObjects(false);

	if (testTime >= ((60.0f / 172.0f) * 2.0f))
	{
		m_fadeValue = 1.0f;
		testTime = 0.0f;
	}

	if (cameraTime >= ((60.0f / 172.0f) * 3.0f))
	{
		++camera;

		if (camera > 3)
			camera = 1;

		m_scene->switchToCamera(camera);
		cameraTime = ((60.0f / 172.0f) * 1.0f);
	}

	m_fadeValue -= deltaTime * 5.0f;

	if (m_fadeValue < 0.0f)
		m_fadeValue = 0.0f;

	testTime += deltaTime;
	cameraTime += deltaTime;

	//if (!m_padCamera)
		m_renderer.setSceneCamera(m_scene->primaryCamera());
	//else
	//	m_renderer.setSceneCamera(&m_padCamera->sceneCamera());

	GS_WRITE(BGCOLOR,GS_BGCOLOR(255,0,0));

	Model::resetCollectedModelCount();
	m_scene->rootNode()->update(time);

	float angle = time * 0.1f;

	Matrix4 my;
	Matrix4 mscale;
	my = my.makeYrotation(angle);

	mscale = Matrix4::identity();
	mscale.setScale(Vector3(1.0f, 1.1f + (m_fadeValue * 2.0f), 1.0f));

	FftStream& stream = FftStream::instance();

	addDisolveObject(stream.bassData(), 1, Matrix4::identity(), 0, 16);
	addDisolveObject(stream.bassData(), 0, my * mscale, 0, 16);

	m_renderer.renderScene(m_scene, true);

	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.glareOffScreen(336, 0.0f, 2, 1.2f, 1.2f);
	}

	if (time < 2.0f)
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeFrame(255, 255, 255, (2.0f - time) * 0.5f);
	}


	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.motionBlur(0x40);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}





