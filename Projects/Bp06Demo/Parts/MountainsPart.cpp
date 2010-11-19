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

#include "MountainsPart.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Geometry/Geometry.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/CubeRenderer.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint m_seed = 93186752;

static const float s_statTime = 11.0f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static float s_syncTimes[] =
{
	16.5f - s_statTime,   // Code

	22.5f - s_statTime,   // emoon
	27.9f - s_statTime,  // Chip

	33.3f - s_statTime,  // Gfx
	38.8f - s_statTime,  // Louie500

	44.5f - s_statTime,  // Music
	50.0f - s_statTime,  // Little Bitchard

	56.0f - s_statTime,  // dummy
	60.0f - s_statTime,  // dummy
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float floatRandom()  
{
	static uint a = 1588635695, m = 4294967291U, q = 2, r = 1117695901;
	m_seed = a*(m_seed % q) - r*(m_seed / q);
	return ((float)m_seed / (float)m);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool MountainsPart::create()
{
	FileSystem& fileSystem = FileSystem::instance();
	InBinarySerializer inBinary;

	FileStream stream;

	if (!fileSystem.open(stream, "DATA/OUTPUT/MONTAINS.ZES"))
		return false;

	if (!inBinary.load(stream))
		return false;

	m_scene = static_cast<SceneInfo*>(inBinary[0]);
	m_renderer.initScene(m_scene);

	// Load credits textures

	m_credits[0] = loadTexture("DATA/OUTPUT/CODE.ZEI");
	m_credits[1] = loadTexture("DATA/OUTPUT/CODEOUT.ZEI");

	m_credits[2] = loadTexture("DATA/OUTPUT/EMOON.ZEI");
	m_credits[3] = loadTexture("DATA/OUTPUT/EMOONOUT.ZEI");

	m_credits[4] = loadTexture("DATA/OUTPUT/CHIP.ZEI");
	m_credits[5] = loadTexture("DATA/OUTPUT/CHIPOUT.ZEI");

	m_credits[6] = loadTexture("DATA/OUTPUT/ART.ZEI");
	m_credits[7] = loadTexture("DATA/OUTPUT/ARTOUT.ZEI");

	m_credits[8] = loadTexture("DATA/OUTPUT/LOUIE.ZEI");
	m_credits[9] = loadTexture("DATA/OUTPUT/LOUIEOUT.ZEI");

	m_credits[10] = loadTexture("DATA/OUTPUT/MUSIC.ZEI");
	m_credits[11] = loadTexture("DATA/OUTPUT/MUSICOUT.ZEI");

	m_credits[12] = loadTexture("DATA/OUTPUT/BITCH.ZEI");
	m_credits[13] = loadTexture("DATA/OUTPUT/BITCHOUT.ZEI");

	m_credits[14] = 0;
/*
	m_credits[1] = loadTexture("DATA/OUTPUT/EMOON.ZEI");
	m_credits[2] = loadTexture("DATA/OUTPUT/CHIP.ZEI");
	m_credits[3] = loadTexture("DATA/OUTPUT/GFX.ZEI");
	m_credits[4] = loadTexture("DATA/OUTPUT/LOUIE500.ZEI");
	m_credits[5] = loadTexture("DATA/OUTPUT/MUSIC.ZEI");
	m_credits[6] = loadTexture("DATA/OUTPUT/BITCHARD.ZEI");
*/
	// Create cube lines

	for (uint j = 0; j < LineCount; ++j)
		m_lines[j] = zenic_new CubeInstance[CubesPerLineCount];

	float yOffsets[] = { 90.0f, 25.0f + 90.0f, 90.0f, 25.0f + 90.0f, 90.0f };  

	float xOffsets[] = { -120.0f + floatRandom() * 0.01f, -70.0f, -30.0f, +20.0f,  70.0f };  
	float shadowOffsets[] = { 0.0f, -0.0f,  0.0f, +0.0f,  0.0f };  

	for (uint j = 0; j < LineCount; ++j)
	{
		float zPos = 5000 + j * 20;
		m_lineSpeed[j] = 30.0f + floatRandom() * 20.0f;

		for (uint i = 0; i < CubesPerLineCount; ++i)
		{
			m_lines[j][i].pos = Vector3(xOffsets[j], yOffsets[j], zPos);
			m_lines[j][i].angleX = floatRandom();
			m_lines[j][i].angleY = floatRandom();
			m_lines[j][i].angleZ = floatRandom();
			m_lines[j][i].rotSpeedX = 0.1f + floatRandom() * 0.9f;
			m_lines[j][i].rotSpeedY = 0.15f + floatRandom() * 0.9f;
			m_lines[j][i].rotSpeedZ = 0.17f + floatRandom() * 0.9f;
			m_lines[j][i].scale = 0.5f + floatRandom() * 0.8f;
			m_lines[j][i].shadowOffset = shadowOffsets[j];
			zPos -= 50.0f + floatRandom() * 40.0f * m_lines[j][i].scale;
		}
	}


	reset();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MountainsPart::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	m_renderer.setClearColor(255, 255, 255);
	m_renderer.setSceneCamera(m_scene->primaryCamera());

	Model::resetCollectedModelCount();
	m_scene->rootNode()->update(time);
	m_renderer.renderScene(m_scene);

	CubeRenderer& cubeRenderer = m_renderer.cubeRenderer();
	cubeRenderer.beginShadow();

	Matrix4 temp;
	temp = temp.identity();

	for (uint j = 0; j < LineCount; ++j)
	{
		for (uint i = 0; i < CubesPerLineCount; ++i)
		{
			temp.setScale(Vector3(0.001f * m_lines[j][i].scale, 
								  0.001f * m_lines[j][i].scale, 
								  0.001f * m_lines[j][i].scale));
			const Vector3& pos = m_lines[j][i].pos;
	
			temp.setPosition(Vector3(pos.x, 60.0f + m_lines[j][i].shadowOffset, pos.z + 15.0f));
			cubeRenderer.renderShadow(temp);
		}
	}
	cubeRenderer.endShadow();

	cubeRenderer.begin(Vector3(0.0f, 0.5f, -0.5f));

	for (uint j = 0; j < LineCount; ++j)
	{
		for (uint i = 0; i < CubesPerLineCount; ++i)
		{
			Matrix4 mx;
			Matrix4 my;
			Matrix4 mz;

			temp.setScale(Vector3(m_lines[j][i].scale, m_lines[j][i].scale, m_lines[j][i].scale));

			cubeRenderer.render(m_lines[j][i].matrix);
	
			mx = mx.makeXrotation(m_lines[j][i].angleX);
			my = my.makeYrotation(m_lines[j][i].angleY);
			mz = mz.makeZrotation(m_lines[j][i].angleZ);
	
			m_lines[j][i].matrix = (mx * my * mz) * temp;
			m_lines[j][i].pos.z -= m_lineSpeed[j] * deltaTime;
	
			if (m_lines[j][i].pos.z < -5000.f)
				m_lines[j][i].pos.z = 5000.0f;
	
			m_lines[j][i].matrix.setPosition(m_lines[j][i].pos);
			m_lines[j][i].angleX += m_lines[j][i].rotSpeedX * deltaTime;
			m_lines[j][i].angleY += m_lines[j][i].rotSpeedX * deltaTime;
			m_lines[j][i].angleZ += m_lines[j][i].rotSpeedX * deltaTime;
		}
	}
	
	cubeRenderer.end();

	m_depthOfFieldPostEffect.render(m_renderer.currentScreen(), m_renderer, m_dmaChain);

	if (time < 3.0f)
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeFrame(255, 255, 255, (3.0f - time) * 0.3f);
	}

	// Show credits screens

	showCredits(time, deltaTime);

	// Fade down

	if (time > (54.83f - s_statTime))
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		float fadeDown = ((55.83f - s_statTime) - time);

		if (fadeDown < 0.0f)
			fadeDown = 0.0f;

		hlp.fadeDownFrame(0, 0, 0, fadeDown);
	}

	if (time > (55.80f - s_statTime))
	{
		m_renderer.setClearColor(0, 0, 0);
	}

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.motionBlur(0x20);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MountainsPart::reset() 
{
	m_syncPos = 0;
	m_tempTime = 0.0f;
	m_currentText = 0;
	m_textPosY = 250.0f;
	m_textPosX = 200.0f;
	m_textAdd = -1.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MountainsPart::showCredits(float time, float deltaTime)
{
	if (!m_credits[m_currentText])
		return;

	if ((time > s_syncTimes[m_syncPos]) && time < (s_syncTimes[m_syncPos] + 1.0f))
	{
		showImage4(m_credits[m_currentText], time - s_syncTimes[m_syncPos], deltaTime);
	}

	if ((time > s_syncTimes[m_syncPos] + 1.0f) && time < (s_syncTimes[m_syncPos+1] - 4.0f))
	{
		showImage4(m_credits[m_currentText], 1.0f, deltaTime);
	}
	if ((time > (s_syncTimes[m_syncPos+1] - 4.0f)) && time < (s_syncTimes[m_syncPos+1] - 3.0f))
	{
		float fadeTime = (((s_syncTimes[m_syncPos+1] - 2.0f) - time) * 0.5f);

		showImage4(m_credits[m_currentText], fadeTime, deltaTime);
		showImage8(m_credits[m_currentText+1], (2.0f - (fadeTime * 2.0f)), deltaTime);
	}

	if ((time > (s_syncTimes[m_syncPos+1] - 3.0f)) && time < (s_syncTimes[m_syncPos+1] - 2.0f))
	{
		float fadeTimeText = (((s_syncTimes[m_syncPos+1] - 2.0f) - time)) - 0.5f;
		float fadeTime = (((s_syncTimes[m_syncPos+1] - 1.0f) - time));

		if (fadeTimeText < 0.0f)
			fadeTimeText = 0.0f;
			
		showImage4(m_credits[m_currentText], fadeTimeText, deltaTime);
		showImage8(m_credits[m_currentText+1], fadeTime - 1.0f, deltaTime);
	}

	if (time > (s_syncTimes[m_syncPos+1] - 2.0f))
	{
		++m_syncPos;
		m_currentText += 2;
		
		if (m_currentText & 1)
		{
			m_textPosY = 150.0f;
			m_textPosX = 150.0f;
			m_textAdd = 1.0f;
		}
		else
		{
			m_textPosY = 250.0f;
			m_textPosX = 200.0f;
			m_textAdd = -1.0f;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MountainsPart::showImage4(const Texture* texture, float strength, float deltaTime)
{
	ZENIC_ASSERT(texture);

	TextureSystem& textureSystem = m_renderer.textureSystem();
	textureSystem.setTexture(0, texture);
	textureSystem.sync();
	textureSystem.activateTexture(m_dmaChain, texture);

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.setRgba(0, 0, 0, 0); 
	hlp.setTex0Pal((450*32) + 32, (450*32), 8, 20, 9, 9, 0, 1);
	hlp.setAlpha(2, 0, 2, 1, (u32)(strength * 128.0f));
	hlp.blit(0, 0, 512, 448, 0.0f, 0.0f, 512.0f, 448.0f, true, true);

	m_textPosX += m_textAdd * deltaTime * 10.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MountainsPart::showImage8(const Texture* texture, float strength, float deltaTime)
{
	ZENIC_ASSERT(texture);

	TextureSystem& textureSystem = m_renderer.textureSystem();
	textureSystem.setTexture(0, texture);
	textureSystem.sync();
	textureSystem.activateTexture(m_dmaChain, texture);

	PostEffectHelper hlp(m_renderer, m_dmaChain);
	hlp.setRgba(0, 0, 0, 0); 
	hlp.setTex0Pal((450*32) + 32, (450*32), 4, 19, 8, 8, 0, 1);
	hlp.setAlpha(2, 0, 2, 1, (u32)(strength * 128.0f));
	hlp.blit(0, 0, 512, 448, 0.0f, 0.0f, 512.0f / 2.0f, 448.0f / 2.0f, true, true);

	m_textPosX += m_textAdd * deltaTime * 10.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture* MountainsPart::loadTexture(const char* filename)
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

