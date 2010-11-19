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

#include "BlobPart.h"
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

bool BlobPart::create()
{
	FileSystem& fileSystem = FileSystem::instance();
	InBinarySerializer inBinary;

	FileStream stream;

	if (!fileSystem.open(stream, "DATA/OUTPUT/BLOB.ZES"))
		return false;

	if (!inBinary.load(stream))
		return false;

	m_scene = static_cast<SceneInfo*>(inBinary[0]);
	m_renderer.initScene(m_scene);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BlobPart::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	static float cameraTime = ((60.0f / 172.0f) * 1.0f);
	static int camera = 1;

	m_renderer.setClearColor(0, 0, 0);

	Model::resetCollectedModelCount();
	m_scene->rootNode()->update(time);
	m_renderer.renderScene(m_scene);

	if (cameraTime >= ((60.0f / 172.0f) * 3.0f))
	{
		++camera;

		if (camera > 3)
			camera = 1;

		m_scene->switchToCamera(camera);
		cameraTime = ((60.0f / 172.0f) * 1.0f);
	}

	cameraTime += deltaTime;

	// Blur glow parts

	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.glareOffScreen(336, 0.0f, 1, 0.8f, 1.6f);
	}


	//if (!m_padCamera)
		m_renderer.setSceneCamera(m_scene->primaryCamera());
	//else
	//	m_renderer.setSceneCamera(&m_padCamera->sceneCamera());

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.motionBlur(0x40);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}







