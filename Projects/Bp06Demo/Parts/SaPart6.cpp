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

#include "SaPart6.h"
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

bool SaPart6::create()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart6::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	static float testTime = 0.0f;
	static int camera = 1;

	ZENIC_ASSERT(m_scene);
	m_renderer.setClearColor(40, 40, 40);

/*
	if (testTime >= ((60.0f / 172.0f) * 2.0f))
	{
		static bool cameraSwitch = false;

		if (cameraSwitch)
			m_scene->switchToCamera(1);
		else
			m_scene->switchToCamera(2);


		cameraSwitch = !cameraSwitch;
		testTime = 0.0f;
	}
*/
	testTime += deltaTime;

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

	addDisolveObject(stream.hihatData(), 0, my);
	addDisolveObject(stream.bassData(), 1, mx);

	m_renderer.renderScene(m_scene, true);

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.motionBlur(0x40);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}






