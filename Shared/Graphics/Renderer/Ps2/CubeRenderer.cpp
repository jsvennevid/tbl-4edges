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

#include "CubeRenderer.h"
#include <Shared/Base/Math/Vector3.h>
#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Geometry/Geometry.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PS2_EMU
extern "C" u32 CubeRendererVu1_CodeStart __attribute__((section(".vudata")));
extern "C" u32 CubeRendererVu1_CodeEnd __attribute__((section(".vudata")));
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

const float CubeScale = 10.0f; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Vector3 s_cubeCoords[] =
{
	Vector3(-1.0f * CubeScale,  1.0f * CubeScale,  1.0f * CubeScale),
	Vector3( 1.0f * CubeScale,  1.0f * CubeScale,  1.0f * CubeScale),
	Vector3( 1.0f * CubeScale, -1.0f * CubeScale,  1.0f * CubeScale),
	Vector3(-1.0f * CubeScale, -1.0f * CubeScale,  1.0f * CubeScale),
	Vector3(-1.0f * CubeScale,  1.0f * CubeScale, -1.0f * CubeScale),
	Vector3( 1.0f * CubeScale,  1.0f * CubeScale, -1.0f * CubeScale),
	Vector3( 1.0f * CubeScale, -1.0f * CubeScale, -1.0f * CubeScale),
	Vector3(-1.0f * CubeScale, -1.0f * CubeScale, -1.0f * CubeScale)
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void CubeRenderer::addQuad(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	m_dmaChain.addFloat(p0.x);
	m_dmaChain.addFloat(p0.y);
	m_dmaChain.addFloat(p0.z);
	m_dmaChain.addFloat(1.0f);

	m_dmaChain.addFloat(p1.x);
	m_dmaChain.addFloat(p1.y);
	m_dmaChain.addFloat(p1.z);
	m_dmaChain.addFloat(1.0f);

	m_dmaChain.addFloat(p2.x);
	m_dmaChain.addFloat(p2.y);
	m_dmaChain.addFloat(p2.z);
	m_dmaChain.addFloat(1.0f);

	m_dmaChain.addFloat(p3.x);
	m_dmaChain.addFloat(p3.y);
	m_dmaChain.addFloat(p3.z);
	m_dmaChain.addFloat(1.0f);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CubeRenderer::CubeRenderer(Renderer& renderer, Dma& dmaChain) : 
	m_renderer(renderer),
	m_dmaChain(dmaChain),
	m_shadowModel(0),
	m_destBuffer1(400),
	m_destBuffer2(800),
	m_started(false),
	m_hasSetMaterial(false)
{
	FileSystem& fileSystem = FileSystem::instance();

	FileStream stream;

	if (!fileSystem.open(stream, "DATA/OUTPUT/CUBESHDW.ZES"))
		return;

	InBinarySerializer inBinary;

	if (!inBinary.load(stream))
		return;

	SceneInfo* scene = static_cast<SceneInfo*>(inBinary[0]);

	// Major ugly but lets keep it this way for now.

	Model::resetCollectedModelCount();
	scene->rootNode()->update(0.0f);

	if (Model::collectedModelsCount() == 1)
		m_shadowModel = Model::collectedModels()[0];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CubeRenderer::begin(const Vector3& directionalLightPosition)
{
	m_started = true;

	m_dmaChain.addSrcCntTag(0, VIF_FLUSH());
	m_dmaChain.endPacket();

	//Vu1::instance()->clearCache();

	// Make sure that everything is finished

	Vu1::instance()->clearCache();

#ifndef PS2_EMU
	m_started = Vu1::instance()->getProgram(m_dmaChain, Vu1::CubeRenderer, 
											&CubeRendererVu1_CodeStart, 
											&CubeRendererVu1_CodeEnd);
#endif


	m_dmaChain.addSrcCntTag(0);
	m_dmaChain.add32(VIF_BASE(100));
	m_dmaChain.add32(VIF_OFFSET(600));
	m_dmaChain.add32(VIF_STCYCL(1, 1));
	m_dmaChain.add32(VIF_MSCAL(m_started));
	m_dmaChain.endPacket();

	// Add cubedata 
	// TODO: Use a refchain here.

	m_dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 1, 22));

	m_dmaChain.addFloat(directionalLightPosition.x);
	m_dmaChain.addFloat(directionalLightPosition.y);
	m_dmaChain.addFloat(directionalLightPosition.z);
	m_dmaChain.addFloat(1.0f);

	m_dmaChain.endPacket();

	m_dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 24, 23));

	addQuad(s_cubeCoords[1], s_cubeCoords[0], s_cubeCoords[3], s_cubeCoords[2]);
	addQuad(s_cubeCoords[4], s_cubeCoords[5], s_cubeCoords[6], s_cubeCoords[7]);
	addQuad(s_cubeCoords[0], s_cubeCoords[1], s_cubeCoords[5], s_cubeCoords[4]);
	addQuad(s_cubeCoords[7], s_cubeCoords[6], s_cubeCoords[2], s_cubeCoords[3]);
	addQuad(s_cubeCoords[5], s_cubeCoords[1], s_cubeCoords[2], s_cubeCoords[6]);
	addQuad(s_cubeCoords[0], s_cubeCoords[4], s_cubeCoords[7], s_cubeCoords[3]);

	m_dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CubeRenderer::render(const Matrix4& matrix)
{
	//ZENIC_ASSERT_DESC(m_started, "Begin hasent been called");

	m_dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACKR(zenic::ps2::vif::V4_32, 4, 0));

	const float* matrixData = matrix.matrix();

	for (uint i = 0; i < 16 ; ++i)
		m_dmaChain.addFloat(*matrixData++);

	m_dmaChain.endPacket();

	// ..

	m_dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACKR(zenic::ps2::vif::V4_32, 1, 4));

	m_dmaChain.add64(GIFTAG(12, 1, 1, GS_PRIM(gs::Triangle, gs::Flat, 0, 0, 0, 0, 0, 
											 gs::Context1, 0), gif::Packed, 4));
	m_dmaChain.add64(0x5551);

	m_dmaChain.add32(VIF_NOP());
	m_dmaChain.add32(VIF_NOP());
	m_dmaChain.add32(VIF_FLUSH());
	m_dmaChain.add32(VIF_MSCNT());

	m_dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CubeRenderer::end()
{
	m_started = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CubeRenderer::beginShadow()
{
	ZENIC_ASSERT(m_shadowModel);

	if (!m_shadowModel)
		return;

	//m_renderer.beginRenderModels();

	// Override the default gifchain with alpha enabled.

	m_dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 1, 20));

	m_dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, gs::Texturing, 0, gs::Blend, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	m_dmaChain.add64(0x512);

	m_dmaChain.endPacket();

	// Set alpha

	m_dmaChain.addSrcCntTag(0, VIF_DIRECT(2));
	m_dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	m_dmaChain.add128(GS_ALPHA(1, 0, 2, 2, 0x80),GS_REG(ALPHA_1));
	m_dmaChain.endPacket();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CubeRenderer::renderShadow(const Matrix4& matrix)
{
	if (!m_shadowModel)
		return;

	if (!m_hasSetMaterial)
	{
		m_renderer.renderModel(m_shadowModel, &matrix, true);
		m_hasSetMaterial = true;
		return;
	}

	m_renderer.renderModel(m_shadowModel, &matrix, false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CubeRenderer::endShadow()
{
	if (!m_shadowModel)
		return;

	m_hasSetMaterial = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

