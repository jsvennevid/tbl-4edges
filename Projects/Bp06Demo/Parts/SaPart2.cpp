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

#include "SaPart2.h"
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

bool SaPart2::create()
{
	loadScene("DATA/OUTPUT/SASCENE2.ZES");
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart2::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	static float cameraTime = 0.0;
	static int camera = 1;

	ZENIC_ASSERT(m_scene);
	m_renderer.setClearColor(40, 40, 40);

	if (cameraTime >= ((60.0f / 172.0f) * 2.0f))
	{
		++camera;

		if (camera > 3)
			camera = 1;

		m_scene->switchToCamera(camera);
		cameraTime = 0.0f;
	}

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
	Matrix4 mx;
	my = my.makeYrotation(angle);
	mx = mx.makeXrotation(angle);

	FftStream& stream = FftStream::instance();

	addDisolveObject(stream.bassData(), 3, Matrix4::identity(), 0, 16);
	addDisolveObject(stream.bassData(), 4, my, 0, 16);

	m_renderer.renderScene(m_scene, true);

	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.glareOffScreen(336, 0.0f, 2, 1.2f, 1.2f);
	}

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.motionBlur(0x40);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}






