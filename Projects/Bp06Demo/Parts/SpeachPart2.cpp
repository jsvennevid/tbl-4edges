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





