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

#include "CityScapePart.h"
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
#include <Shared/Graphics/Renderer/Ps2/Branch/Branch.h>
#include <Shared/Graphics/Renderer/Ps2/Branch/BranchGenerator.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CityScapePart::create()
{
	FileSystem& fileSystem = FileSystem::instance();
	InBinarySerializer inBinary;

	GS_WRITE(BGCOLOR,GS_BGCOLOR(255,0,0));

	FileStream stream;

	if (!fileSystem.open(stream, "DATA/OUTPUT/CITY.ZES"))
		return false;

	if (!inBinary.load(stream))
		return false;

	m_scene = static_cast<SceneInfo*>(inBinary[0]);

	if (!fileSystem.open(stream, "DATA/OUTPUT/SCREDGEA.ZEI"))
		return false;

	if (!inBinary.load(stream))
		return false;

	m_alphaMask = static_cast<Texture*>(inBinary[0]);

	// move into the alpha value

	u8* data = m_alphaMask->palette().objects();

	for (uint i = 0; i < 255; ++i)
	{
		data[0] = data[1];
		data[3] = data[1];
		data += 4;
	}

	m_branchSettings.m_length = 300.0f;
	m_branchSettings.m_lengthVariation = 100.0f;

	m_branchSettings.m_angle = 17.0f;
	m_branchSettings.m_angleVariation = 10.0f;

	m_branchSettings.m_startRadius = 20.0f;
	m_branchSettings.m_startRadiusVariation = 0.2f;

	m_branchSettings.m_contact = 101.0f;
	m_branchSettings.m_contactVariation = 0.01f;

	m_branchSettings.m_gravity = -0.01f;
	m_branchSettings.m_gravityVariation = 0.0f;

	m_branchSettings.m_speed = 140.3f;
	m_branchSettings.m_sides = 4+1;

	Branch::s_settings = &m_branchSettings;

	Branch::init();

	BranchGenerator::m_branchIndex = 0;
	BranchGenerator::m_stripIndex = 0;

	m_renderer.initScene(m_scene);


	for (uint i = 0; i < MaxCount; ++i)
	{
		m_branches[i] = zenic_new Branch;
		m_branches[i]->createRoot(Vector3(0 + (i * 100 + (rand() % 50)), 800, -150.0f), Vector3(0.0f, 0.0f, -1.0f));
		m_branches[i]->divide();
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CityScapePart::reset()
{
	BranchGenerator::m_branchIndex = 0;
	BranchGenerator::m_stripIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CityScapePart::update(float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	ZENIC_ASSERT(m_scene);

	GS_WRITE(BGCOLOR,GS_BGCOLOR(255,0,0));

	m_renderer.setClearColor(255, 255, 255);
	m_renderer.setSceneCamera(m_scene->primaryCamera());

	Model::resetCollectedModelCount();
	m_scene->rootNode()->update(time);
	m_renderer.renderScene(m_scene);

	if (m_divideTimer > 2.0f)
	{
		for (uint i = 0; i < MaxCount; ++i)
			m_branches[i]->divide();

		m_divideTimer = 0.0f;
	}
/*
	if (m_splitTimer > 10.0f)
	{
		for (uint i = 0; i < 10; ++i)
			m_branches[i]->split();

		m_splitTimer = 0.0f;
	}
*/
	m_divideTimer += deltaTime;

	s32 start = Branch::beginRender(m_dmaChain);

	m_dmaChain.addSrcCntTag(0, VIF_UNPACK(vif::V4_32, 4, 10));

	// Temporay identity matrix

	Matrix4 temp;
	temp = temp.identity();

	float* matrix = temp.matrix();

	for (uint i = 0; i < 16; i++)
		m_dmaChain.addFloat(*matrix++);

	m_dmaChain.endPacket();

	m_dmaChain.addSrcCntTag();
	m_dmaChain.add32(0);
	m_dmaChain.add32(0);
	m_dmaChain.add32(VIF_FLUSH());
	m_dmaChain.addMscal(start);
	m_dmaChain.endPacket();

	for (uint i = 0; i < MaxCount; ++i)
		m_branches[i]->render(m_dmaChain, deltaTime);

	m_fadeToFogPostEffect.render(255, 255, 255, m_renderer.currentScreen(), m_dmaChain);

	if (time < 2.0f)
	{
		PostEffectHelper hlp(m_renderer, m_dmaChain);
		hlp.fadeFrame(255, 255, 255, (2.0f - time) * 0.5f);
	}

	m_renderer.renderScreenBlur(m_alphaMask);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}


