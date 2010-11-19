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

#include "ParkingHousePart.h"
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParkingHousePart::create()
{
	FileSystem& fileSystem = FileSystem::instance();
	InBinarySerializer inBinary;

	FileStream stream;

	if (!fileSystem.open(stream, "DATA/OUTPUT/PARKHOSE.ZES"))
		return false;

	if (!inBinary.load(stream))
		return false;

	m_scene = static_cast<SceneInfo*>(inBinary[0]);
	m_renderer.initScene(m_scene);

	m_scene->switchToCamera(3);

	m_textures[0] = loadTexture("DATA/OUTPUT/G1.ZEI");
	m_textures[1] = loadTexture("DATA/OUTPUT/G2.ZEI");
	m_textures[2] = loadTexture("DATA/OUTPUT/G3.ZEI");
	m_textures[3] = loadTexture("DATA/OUTPUT/G4.ZEI");
	m_textures[4] = loadTexture("DATA/OUTPUT/G5.ZEI");
	m_textures[5] = loadTexture("DATA/OUTPUT/G6.ZEI");
	m_textures[6] = loadTexture("DATA/OUTPUT/G7.ZEI");
	m_textures[7] = loadTexture("DATA/OUTPUT/G8.ZEI");
	m_textures[8] = 0;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParkingHousePart::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	static float cameraTime = 0.0;
	static float flashTime = 0.0;
	static float flashCounter = 0.0;
	static int camera = 3;
	static float beatTime = ((60.0f / 172.0f) * 2.0f);

	GS_WRITE(BGCOLOR,GS_BGCOLOR(255,0,0));

	ZENIC_ASSERT(m_scene);
	m_renderer.setClearColor(253, 175, 42);
	m_renderer.camera().setViewWindowSize(1.8f);
	m_renderer.camera().setMaxZ(0x4ffff);

	Model::resetCollectedModelCount();
	m_scene->rootNode()->update(time);
	m_renderer.renderScene(m_scene);


	if (flashTime >= ((60.0f / 172.0f) * 2.0f))
	{
		flashCounter = 1.0f;
		flashTime = 0.0f;
	}


	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeFrame(255, 255, 255, flashCounter);
	}

	cameraTime += deltaTime;
	flashTime += deltaTime;
	flashCounter -= deltaTime * 2.0f;

	if (time < 22.31f)
	{

		if (cameraTime >= ((60.0f / 172.0f) * 16.0f))
		{
			if (camera == 1)
				camera = 3;
			else
				camera = 1;
	
			m_scene->switchToCamera(camera);
			ZENIC_INFO("camera " << camera);
	
			cameraTime = 0.0f;
		}
	}
	else
	{
		m_scene->switchToCamera(4);

		if (m_currentGreeting < 8)
		{
			if (beatTime >= ((60.0f / 172.0f) * 8.0f))
			{
				++m_currentGreeting;
				beatTime = 0.0f;
			}
	
			showImage4(m_textures[m_currentGreeting], 1.0f);
			beatTime += deltaTime;
		}
	}

	m_renderer.setSceneCamera(m_scene->primaryCamera());

	//m_renderer.setSceneCamera(&camera);
	//m_renderer.setSceneCamera(&camera);

	//if (!m_padCamera)
	//else
	//	m_renderer.setSceneCamera(&m_padCamera->sceneCamera());

	
	{
		//PostEffectHelper hlp(m_renderer, m_dmaChain);
		//hlp.glareOffScreen(336, 0.0f, 1, 0.5f, 0.6f);
	}

	m_fadeToFogPostEffect.render(253, 175, 42, m_renderer.currentScreen(), m_dmaChain);

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.motionBlur(0x20);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture* ParkingHousePart::loadTexture(const char* filename)
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

void ParkingHousePart::showImage4(const Texture* texture, float strength)
{
	if (!texture)
		return;

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


	}
}



