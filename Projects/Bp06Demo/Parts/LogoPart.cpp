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

#include "LogoPart.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Geometry/Geometry.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/CubeRenderer.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

uint m_seed2 = 93186752;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float floatRand()
{
	static uint a = 1588635695, m = 4294967291U, q = 2, r = 1117695901;
	m_seed2 = a*(m_seed2 % q) - r*(m_seed2 / q);
	return ((float)m_seed2 / (float)m);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool LogoPart::create()
{
	FileSystem& fileSystem = FileSystem::instance();

	FileStream stream;

	if (!fileSystem.open(stream, "DATA/OUTPUT/CUBICLOG.RAW"))
		return false;

	stream.read(m_logo, LogoSize);
	stream.close();

	// count "cubes"

	m_cubeCount = 0;

	for (uint i = 0; i < LogoSize; ++i)
	{
		if (m_logo[i] != 0)
			++m_cubeCount;
	}

	m_cubeCount *= 16;

	ZENIC_INFO("LogoPart: cubeCount " << m_cubeCount);

	m_cubes = zenic_new Cube[m_cubeCount];
	uint cubeIndex = 0;

	for (uint y = 0; y < LogoY; ++y)
	{
		for (uint x = 0; x < LogoX; ++x)
		{
			if (m_logo[(y * LogoX) + x] == 0)
				continue;

			for (uint offsetY = 0; offsetY < 4; ++offsetY)
			{
				for (uint offsetX = 0; offsetX < 4; ++offsetX)
				{
					float xPos = (-320.0f) + ((x + offsetX * 0.25f) * 10.0f); 
					float yPos = (float)(LogoY - (y + offsetY * 0.25f)) * 10.0f;

					//if (y == 0)
					//	ZENIC_INFO("x " << x);

					m_cubes[cubeIndex].pos = Vector3(xPos, yPos, 0.0f);
					m_cubes[cubeIndex].rot.x = floatRand();
					m_cubes[cubeIndex].rot.y = floatRand();
					m_cubes[cubeIndex].rot.z = floatRand();
					m_cubes[cubeIndex].rotSpeed.x = 0.1f + floatRand() * 0.9f;
					m_cubes[cubeIndex].rotSpeed.y = 0.1f + floatRand() * 0.9f;
					m_cubes[cubeIndex].rotSpeed.z = 0.1f + floatRand() * 0.9f; 
					m_cubes[cubeIndex].direction.x = 0.5f - floatRand() * 0.5f;
					m_cubes[cubeIndex].direction.y = -(0.5f - floatRand() * 0.5f);
					m_cubes[cubeIndex].direction.z = 0.5f - floatRand() * 0.5f;
					m_cubes[cubeIndex].direction = m_cubes[cubeIndex].direction.normalize();
					m_cubes[cubeIndex].speed = 60 + floatRand() * 40.0f;
					++cubeIndex;
				}
			}
/*
			m_cubes[cubeIndex].pos = Vector3((-32 * 10.0f) + (float)x * 10.0f, (float)(LogoY - y) * 10.0f, 0.0f);
			m_cubes[cubeIndex].direction.x = 0.5f - floatRand() * 0.5f;
			m_cubes[cubeIndex].direction.y = -0.5f + floatRand() * 0.2f;
			m_cubes[cubeIndex].direction.z = 0.5f - floatRand() * 0.5f;
			m_cubes[cubeIndex].direction = m_cubes[cubeIndex].direction.normalize();
			m_cubes[cubeIndex].speed = 30 + floatRand() * 10.0f;
			++cubeIndex;
*/
		}
	}


	ZENIC_INFO("cubeIndex " << cubeIndex);

	Matrix4 cameraMatrix;
	cameraMatrix = cameraMatrix.identity();
	cameraMatrix.setPosition(Vector3(-30, 460, 420));
	m_sceneCamera.setTransform(cameraMatrix);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LogoPart::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	m_renderer.setClearColor(255, 255, 255);

	CubeRenderer& cubeRenderer = m_renderer.cubeRenderer();
	m_renderer.setSceneCamera(&m_sceneCamera);

	cubeRenderer.begin(Vector3(0.0f, 0.5f, 0.5f));


	Matrix4 cameraMatrix;
	cameraMatrix = cameraMatrix.identity();
	cameraMatrix.setPosition(Vector3(-30, 460, m_zPos));
	m_sceneCamera.setTransform(cameraMatrix);

	m_zPos -= 10.0f * deltaTime;

	Matrix4 scaleMatrix;
	scaleMatrix = scaleMatrix.identity();
	scaleMatrix.setScale(Vector3(0.26f, 0.26f, 0.26f));

	if (time > 3.0f)
	{
		for (uint i = 0; i < m_cubeCount; ++i)
		{
			Matrix4 mx;
			Matrix4 my;

			Vector3& pos = m_cubes[i].pos;
			Vector3& direction = m_cubes[i].direction;

			pos.x += direction.x * m_cubes[i].speed * deltaTime;
			pos.y += direction.y * m_cubes[i].speed * deltaTime;
			pos.z += direction.z * m_cubes[i].speed * deltaTime;

			mx = mx.makeXrotation(m_cubes[i].rot.x);
			my = my.makeYrotation(m_cubes[i].rot.y);
			//mz = mz.makeZrotation(m_cubes[i].rot.z);

			Matrix4 matrix = (mx * my) * scaleMatrix;
			matrix.setPosition(pos);

			m_cubes[i].rot.x += m_cubes[i].rotSpeed.x * deltaTime;
			m_cubes[i].rot.y += m_cubes[i].rotSpeed.y * deltaTime;
			m_cubes[i].rot.z += m_cubes[i].rotSpeed.z * deltaTime;
				
			cubeRenderer.render(matrix);
		}
	}
	else
	{
		Matrix4 scaleMatrix;
		scaleMatrix = scaleMatrix.identity();
		scaleMatrix.setScale(Vector3(0.26f, 0.26f, 0.26f));

		for (uint i = 0; i < m_cubeCount; ++i)
		{
			scaleMatrix.setPosition(m_cubes[i].pos);
			cubeRenderer.render(scaleMatrix);
		}
	}
	

	cubeRenderer.end();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}



