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

#include "SaPart3.h"
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

bool SaPart3::create()
{
	loadScene("DATA/OUTPUT/SASCENE3.ZES");
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart3::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
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
	mscale.setScale(Vector3(1.0f, 1.0f, 1.1f + (m_fadeValue * 2.0f)));

	FftStream& stream = FftStream::instance();

	addDisolveObject(stream.bassData(), 5, my, 0, 20);
	addDisolveObject(stream.bassData(), 6, mscale, 0, 20);

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






