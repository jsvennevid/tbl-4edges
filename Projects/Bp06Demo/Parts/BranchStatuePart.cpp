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

#include "BranchStatuePart.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Graphics/Renderer/DebugRenderer.h>
#include <Shared/Graphics/Renderer/Ps2/Material.h>
#include <Shared/Geometry/Geometry.h>

#include <Shared/Hardware/Ps2/Dma.h>

#include <Shared/Geometry/Ps2/BranchModel.h>
#include <Shared/Graphics/Renderer/Ps2/Branch/Branch.h>
#include <Shared/Graphics/Renderer/Ps2/Branch/BranchGenerator.h>

#ifdef PS2_EMU
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

ZENIC_REFERENCE_SERIALIZABLE_FACTORY(Material); // Temporary

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool BranchStatuePart::create()
{
	FileSystem& fileSystem = FileSystem::instance();
	
	FileStream stream;
	
	if (!fileSystem.open(stream, "Data/Output/BranchStatue.zse"))
		return false;

	InBinarySerializer inBinary;

	if (!inBinary.load(stream))
		return false;

	m_scene = static_cast<SceneInfo*>(inBinary[0]);

	m_branchSettings.m_length = 1.0f;
	m_branchSettings.m_lengthVariation = 0.5f;

	m_branchSettings.m_angle = 25.0f;
	m_branchSettings.m_angleVariation = 15.0f;

	m_branchSettings.m_startRadius = 0.05f;
	m_branchSettings.m_startRadiusVariation = 0.01f;

	m_branchSettings.m_contact = 99.0f;
	m_branchSettings.m_contactVariation = 0.01f;

	m_branchSettings.m_gravity = -0.2;
	m_branchSettings.m_gravityVariation = 0.0f;

	m_branchSettings.m_speed = 0.3f;
	m_branchSettings.m_sides = 4+1;

	Branch::s_settings = &m_branchSettings;

	Branch::init();

	BranchGenerator::m_branchIndex = 0;
	BranchGenerator::m_stripIndex = 0;

	// Temp temp

	Model::resetCollectedModelCount();

	ZENIC_ASSERT(m_scene->rootNode());
	m_scene->rootNode()->update(0.0f);

	Model** models = Model::collectedModels();
	//uint modelCount = Model::collectedModelsCount();

	// Just get the first model for now

	BranchModel* model = static_cast<BranchModel*>(models[0]);
	const DataPtr<u16>& curveCounts = model->curvesCount();

	m_branch = zenic_new Branch[curveCounts.count()];
	ZENIC_ASSERT(m_branch);

	BranchModel::Vertex* vertices = model->curves().objects();
	BranchModel::Vertex* verticesStart = model->curves().objects();
	u16* curveCountArray = curveCounts.objects();

	BranchModel::Vertex* verticesPrev = vertices;
	vertices += curveCountArray[0];
	vertices += curveCountArray[0];

	for (uint i = 0; i < 4; ++i)
	{
		//if ((i + 1) == curveCounts.count())
		//	vertices = verticesStart;

		m_branch[i].setControlPoints(verticesPrev, vertices, curveCountArray[i], i * 0.2f, model->scale());
		m_branch[i].createRoot();
		m_branch[i].divide();
	}

	m_branchSettings.m_speed = 1.0f;
	m_splitCounter = 0.0f;


	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BranchStatuePart::update(Dma& dmaChain, float time, float deltaTime, SyncPoints::Part* syncPoints)
{
	if (m_splitCounter > 1.0f)
	{
		for (uint i = 0; i < 4; ++i)
			m_branch[i].divide();

		m_splitCounter = 0.0f;
	}

	Model::resetCollectedModelCount();

	ZENIC_ASSERT(m_scene->rootNode());
	m_scene->rootNode()->update(time);

	Model** models = Model::collectedModels();
	uint modelCount = Model::collectedModelsCount();

	//m_branch->setModel(static_cast<BranchModel*>(models[0]));

#ifdef PS2_EMU
	glBegin(GL_LINE_STRIP);
	//glClearColorf(0.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);

	float fade = 1.0f;

	for (uint k = 0; k < modelCount; ++k)
	{
		const BranchModel* model = static_cast<const BranchModel*>(models[k]);
		const DataPtr<BranchModel::Vertex>& curves = model->curves();

		BranchModel::Vertex* objects = curves.objects();
		const Vector3& scale = model->scale();

		for (uint i = 0; i < 14; ++i)
		{
			Vector3 pos(objects[i].x * scale.x, objects[i].y * scale.y, objects[i].z * scale.z);
			glVertex3f(pos.x, pos.y, pos.z);
			if (i & 1)
				glColor3f(0, 1, 0.0f);
			else
				glColor3f(1.0f, 0, 0.0f);
		}
	}

	glEnd();

#endif

	s32 start = Branch::beginRender(dmaChain);

	dmaChain.addSrcCntTag(0, VIF_UNPACK(vif::V4_32, 4, 10));

	// Temporay identity matrix

	Matrix4 temp;
	temp = temp.identity();

	float* matrix = temp.matrix();

	for (uint i = 0; i < 16; i++)
		dmaChain.addFloat(*matrix++);

	dmaChain.endPacket();

	dmaChain.addSrcCntTag();
	dmaChain.add32(0);
	dmaChain.add32(0);
	dmaChain.add32(VIF_FLUSH());
	//dmaChain.add32(0);
	dmaChain.addMscal(start);
	dmaChain.endPacket();

	for (uint i = 0; i < 4; ++i)
		m_branch[i].render(dmaChain, deltaTime);

	m_splitCounter += deltaTime;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
