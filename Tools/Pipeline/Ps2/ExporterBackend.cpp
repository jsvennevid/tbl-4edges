///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005-2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ExporterBackend.h"
#include "TextureBuilder.h"
#include "ShaderBuilder.h"
#include "../TriangleStrip.h"

#include <Shared/Geometry/Ps2/ModelData.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Graphics/Renderer/Ps2/Material.h>
#include <Shared/Graphics/Image/QuantizerHistogram.h>
#include <Shared/Graphics/Scene/Bound.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Base/Storage/String.h>

#pragma warning(disable : 4505 4100 4996 4244)

#include <windows.h>
#include <Shape.h>
#include <Model.h>
#include <XSIMesh.h>
#include <XSIShape.h>
#include <XSIImage.h>
#include <Mesh.h>
#include <SIBCPixMap.h> 
#include <TriangleList.h>
#include <XSIMesh.h>
#include <XSITriangleList.h>
#include <GlobalMaterial.h>
#include "../DegenerateMesh.h"
#include "BranchModelBuilder.h"
#include "DotModelBuilder.h"

#pragma warning(default : 4505 4100 4996 4244)  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint ExporterBackend::m_vu1HeaderSize = 24;
ExporterBackend::MeshVertex ExporterBackend::m_buildChunk[256];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

zenic::Model* ExporterBackend::buildModel(CSLModel* model)
{
	Model* modelNode = 0;

	// Temporay hack, buid special model if we have "branchmodel_" prefix

	std::string name = model->GetName();

	if (!name.find("branchmodel_"))
	{
		BranchModelBuilder builder;
		modelNode = builder.build(model);
	} 
	else if(!name.find("dotmodel_"))
	{
		DotModelBuilder builder;
		modelNode = builder.build(model);
	}
	else
	{
		modelNode = zenic_new Model;

		buildNode(model, modelNode);

		modelNode->setModelData(buildModelData(model, modelNode));
	}

	return modelNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Serializable* ExporterBackend::buildTexture(CSLImage* image, CSIBCPixMap& pixMap)
{
	TextureBuilder builder;
	Texture* texture = 0;

	uint depth = pixMap.GetTotalPixelDepthByte();

	// TODO: Fix me, move this to a separate function instead.

	if (depth == 3)
	{
		u8* convImage = 0;
		u8* convImageTemp = 0;
		u8* srcImage = (u8*)pixMap.GetMap();
		uint size = pixMap.GetWidth() * pixMap.GetHeight();
		convImage = convImageTemp = zenic_new u8[size * 4];
		
		for (uint i = 0; i < size; ++i)
		{
			*convImageTemp++ = *srcImage++;
			*convImageTemp++ = *srcImage++;
			*convImageTemp++ = *srcImage++;
			*convImageTemp++ = 0xff;
		}

		texture = builder.createTexture(pixMap.GetWidth(), pixMap.GetHeight(), gs::CT32, gs::T8, 1, convImage);

		zenic_delete [] convImage;
	}
	else
	{
		texture = builder.createTexture(pixMap.GetWidth(), pixMap.GetHeight(), gs::CT32, gs::T8, 1, (u8*)pixMap.GetMap());
	}

	return texture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

zenic::Material* ExporterBackend::buildMaterial(CSLXSIMaterial* xsiMaterial)
{
	ps2::Material* material = zenic_new ps2::Material;

	ps2::ShaderBuilder::build(material, *this, xsiMaterial);

	return material;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

zenic::ModelData* ExporterBackend::buildModelData(CSLModel* model, Model* modelNode)
{
	ModelData* target = zenic_new ModelData;

	CSLXSIMesh* mesh = static_cast<CSLXSIMesh*>(model->Primitive());
	CSLXSIShape* shape = mesh->XSIShape();

	DegenerateMesh degMesh;

	// Temp temp

	if (strstr(shape->GetName(), "spectrum"))
		degMesh.process(mesh, true);
	else
		degMesh.process(mesh);

	TriangleStrip triangleStripper(TriangleStrip::Actc);

	const TriangleStrip::StripInfo& stripInfo = triangleStripper.process(&degMesh);

	// TODO: Make this a bit cleaner

	CSLXSITriangleList* triangleList = mesh->XSITriangleLists()[0];
	ZENIC_ASSERT_DESC(triangleList, "No triangles in the model ");

	CSLBaseMaterial* xsiMaterial = triangleList->GetMaterial();

	ps2::Material* material = (ps2::Material*)findMaterial(xsiMaterial);
	target->setMaterial(material);

	ZENIC_ASSERT_DESC(material, "Must have proper material");

	if (!strstr(shape->GetName(), "spectrum"))
		calculateBound(shape, modelNode);

	// Set the correct material to the modelData

	char* temp = new char[1024*1024]; // should be enough
	Dma dmaChain(temp,(1024*1024));

	m_chunkCount = 0;
	setupVertexFormat(shape);	
	setupShaderPipeline(target, shape->GetName());
	
	// Generate trianglestrips

	dmaChain.addSrcRetTag(0);

	Color32 color = material->color();

	const std::vector<DegenerateMesh::Vertex>& vertices = degMesh.vertexList();

	for (std::vector<DataPtr<u32>*>::const_iterator i = stripInfo.stripList.begin(); i != stripInfo.stripList.end(); ++i)
	{
		DataPtr<u32>* triStripList = (*i);
	
		u32* stripList = triStripList->objects();
		u32 stripCount = triStripList->count();

		for (uint d = 0; d < stripCount; ++d)
		{
			const DegenerateMesh::Vertex& vertex = vertices[stripList[d]];

			addVertex(dmaChain, vertex.position, color,
					  vertex.normal, vertex.uv.x, vertex.uv.y, d < 2);
		}
	}

	flushChunk(dmaChain);

	dmaChain.add32(VIF_STCYCL(1,1));
	dmaChain.add32(VIF_FLUSH());		// temporary
	dmaChain.align(16);
	dmaChain.endPacket();

	getChain(target, dmaChain);

	return target;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Take care of material/shader setting to see what kind of data we need for rendering the object.

void ExporterBackend::setupVertexFormat(CSLXSIShape* shape)
{
	m_chunkSize = Vu1ChunkSize - m_vu1HeaderSize/2;

	if (strstr(shape->GetName(), "spectrum"))
		m_vertexFormat = Position;
	else
		m_vertexFormat = PositionInt;

	m_cycleFormat = 0x101;
	m_vertexOffset = 1;

	// Always included colors now (used for adc)

	m_vertexFormat |= Color;
	++m_cycleFormat;
	++m_vertexOffset;
	m_chunkSize >>= 1;

	if (strstr(shape->GetName(), "spectrum"))
	{
		if (shape->GetNormalListCount() > 0)
		{
			m_vertexFormat |= Normal;
			++m_cycleFormat;
			++m_vertexOffset;
			m_chunkSize >>= 1;
		}
	}

	if (shape->GetTexCoordListCount() > 0)
	{
		m_vertexFormat |= Texcoord;
		++m_cycleFormat;
		++m_vertexOffset;
		m_chunkSize >>= 1;
	}
	
	m_chunkSize /= 3;	// 3 buffers inside each vif buffer
	m_chunkSize -= 1;	// for giftag at the begining of the kickbuffer
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ExporterBackend::getChain(ModelData* target,Dma& dmaChain)
{
	DataPtr<u8> chain;

	uint chainSize = dmaChain.calcSize();
	chain.allocate(chainSize);
	u8* chainData = chain.objects();

	memcpy(chainData, dmaChain.getData(), chainSize);

	ZENIC_INFO("Size of chain: " << chainSize);

	target->setPrimaryChain(chain);
	target->setType(ModelData::Normal);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ExporterBackend::flushChunk(Dma& dmaChain)
{
	// TODO: Fix so we dont assume vertexcolors and make vertex unpack using filling write

	if (m_chunkCount <= 2)
		return;

	// info for each chunk

	dmaChain.add32(VIF_ITOP(m_chunkCount));
	dmaChain.add32(VIF_STCYCL(1,1));
	dmaChain.add32(VIF_UNPACKRU(vif::V4_16, 1, 0));

	u16 offset = u16(((m_chunkCount * m_vertexOffset * 2) + 1)); 	// kickBuffer + 1 to save one instruction on the vu

	dmaChain.add16(u16(m_vertexOffset));				
	dmaChain.add16(u16(m_chunkCount * m_vertexOffset));				// tempBuffer
	dmaChain.add16(offset);
	dmaChain.add16(0);

	// TODO: Remove cycle setting and do it once per mesh instead.

	if (m_vertexFormat & Position)
	{
		dmaChain.add32(VIF_CODE(vif::Stcycl, 0, m_cycleFormat));
		dmaChain.add32(VIF_UNPACKR(vif::V4_32, m_chunkCount, 1));

		for (uint i = 0; i < m_chunkCount; ++i)
		{
			dmaChain.addFloat(m_buildChunk[i].vertex.x);
			dmaChain.addFloat(m_buildChunk[i].vertex.y);
			dmaChain.addFloat(m_buildChunk[i].vertex.z);
			dmaChain.addFloat(1.0f);
		}
	}

	if (m_vertexFormat & PositionInt)
	{
		dmaChain.add32(VIF_CODE(vif::Stcycl, 0, m_cycleFormat));
		dmaChain.add32(VIF_UNPACKR(vif::V3_16, m_chunkCount, 1));

		for (uint i = 0; i < m_chunkCount; ++i)
		{
			float x = m_buildChunk[i].vertex.x * m_maxScale.x;
			float y = m_buildChunk[i].vertex.y * m_maxScale.y;
			float z = m_buildChunk[i].vertex.z * m_maxScale.z;
		
			ZENIC_ASSERT_DESC(fabs(x) < 32768.0f, "Vertices too big");
			ZENIC_ASSERT_DESC(fabs(y) < 32768.0f, "Vertices too big");
			ZENIC_ASSERT_DESC(fabs(z) < 32768.0f, "Vertices too big");

			dmaChain.add16((s16)x);
			dmaChain.add16((s16)y);
			dmaChain.add16((s16)z);
		}
	}

	dmaChain.align(4);
	
	//if (m_vertexFormat & Color)
	{
		dmaChain.add32(VIF_CODE(vif::Stcycl, 0, m_cycleFormat));
		dmaChain.add32(VIF_UNPACKR(vif::V4_5, m_chunkCount, 2));

		for (uint i = 0; i < m_chunkCount; ++i)
		{
			uint r = (uint)(m_buildChunk[i].color.r >> 3);  
			uint g = (uint)(m_buildChunk[i].color.g >> 3); 
			uint b = (uint)(m_buildChunk[i].color.b >> 3); 
			dmaChain.add16(u16((b << 10) | (g << 5) | (r << 0) | (m_buildChunk[i].adc ? 0x8000 : 0)));
		}
	}

	dmaChain.align(4);

	if ((m_vertexFormat & Texcoord) == Texcoord)
	{
		dmaChain.add32(VIF_CODE(vif::Stcycl, 0, m_cycleFormat));
		dmaChain.add32(VIF_UNPACKR(vif::V2_16, m_chunkCount, 3));

		for (uint i = 0; i < m_chunkCount; ++i)
		{
			dmaChain.add16((s16)(m_buildChunk[i].s * 4096.0f)); 
			dmaChain.add16((s16)(m_buildChunk[i].t * 4096.0f)); 
			//dmaChain.addFloat(m_buildChunk[i].s); 
			//dmaChain.addFloat(m_buildChunk[i].t); 
		}
	}

	dmaChain.align(4);

	dmaChain.align(4);

	if ((m_vertexFormat & Normal) == Normal)
	{
		dmaChain.add32(VIF_CODE(vif::Stcycl, 0, m_cycleFormat));
		dmaChain.add32(VIF_UNPACKR(vif::V3_16, m_chunkCount, 4));

		for (uint i = 0; i < m_chunkCount; ++i)
		{	
			// just to be sure.
			m_buildChunk[i].normal = m_buildChunk[i].normal.normalize();

			float x = m_buildChunk[i].normal.x * 32767.0f;
			float y = m_buildChunk[i].normal.y * 32767.0f;
			float z = m_buildChunk[i].normal.z * 32767.0f;

			ZENIC_ASSERT_DESC(fabs(x) < 32768.0f, "Vertices too big");
			ZENIC_ASSERT_DESC(fabs(y) < 32768.0f, "Vertices too big");
			ZENIC_ASSERT_DESC(fabs(z) < 32768.0f, "Vertices too big");

			dmaChain.add16((s16)(x)); 
			dmaChain.add16((s16)(y)); 
			dmaChain.add16((s16)(z)); 
		}
	}

	dmaChain.align(4);

	dmaChain.add32(VIF_ITOP(m_chunkCount));
	dmaChain.add32(VIF_MSCNT());

	m_buildChunk[0] = m_buildChunk[m_chunkCount - 2];
	m_buildChunk[1] = m_buildChunk[m_chunkCount - 1];
	m_buildChunk[0].adc = true;
	m_buildChunk[1].adc = true;

	m_chunkCount = 2;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ExporterBackend::addVertex(Dma& dmaChain, const Vector3& vertex, const Color32& color, const Vector3& normal,
								const float& s, const float& t, const uint& adc)
{
	m_buildChunk[m_chunkCount].vertex = vertex;
	m_buildChunk[m_chunkCount].color = color;
	m_buildChunk[m_chunkCount].normal = normal;
	m_buildChunk[m_chunkCount].s = s;
	m_buildChunk[m_chunkCount].t = t;
	m_buildChunk[m_chunkCount].adc = adc;
	m_chunkCount++;

	if (m_chunkCount > m_chunkSize-1)
		flushChunk(dmaChain);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ExporterBackend::calculateBound(CSLXSIShape* shape, zenic::Model* modelNode)
{
	const Vector3* vertexArray = (Vector3*)shape->GetVertexPositionList()->GetAttributeArray()->ArrayPtr();
	const uint arrayCount = static_cast<uint>(shape->GetVertexPositionList()->GetCount());

	Vector3 maxValue(-100000000.0f, -100000000.0f, -100000000.0f); 
	Vector3 minValue(100000000.0f, 100000000.0f, 100000000.0f); 

	for (uint i = 0; i < arrayCount; ++i)
	{
		if (vertexArray[i].x < minValue.x)
			minValue.x = vertexArray[i].x;

		if (vertexArray[i].x > maxValue.x)
			maxValue.x = vertexArray[i].x;

		if (vertexArray[i].y < minValue.y)
			minValue.y = vertexArray[i].y;

		if (vertexArray[i].y > maxValue.y)
			maxValue.y = vertexArray[i].y;

		if (vertexArray[i].z < minValue.z)
			minValue.z = vertexArray[i].z;

		if (vertexArray[i].z > maxValue.z)
			maxValue.z = vertexArray[i].z;
	}
	
	float xScale = fabs(maxValue.x) + fabs(minValue.x);
	if (xScale <= 0.0f)
		xScale = 1.0f;

	float yScale = fabs(maxValue.y) + fabs(minValue.y);
	if (yScale <= 0.0f)
		yScale = 1.0f;

	float zScale = fabs(maxValue.z) + fabs(minValue.z);
	if (zScale <= 0.0f)
		zScale = 1.0f;

	m_maxScale = Vector3(32760.0f / xScale, 32760.0f / yScale, 32760.0f / zScale);

	// Setup the scaling values (used for 16 bit integer vertices)

	Matrix4 transform = modelNode->transform();
	transform.setScale(Vector3(1.0f / m_maxScale.x, 1.0f / m_maxScale.y, 1.0f / m_maxScale.z));
	modelNode->setScale(Vector3(1.0f / m_maxScale.x, 1.0f / m_maxScale.y, 1.0f / m_maxScale.z));
	modelNode->setTransform(transform);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ExporterBackend::setupShaderPipeline(ps2::ModelData* modelData, const char* name)
{
	// TODO: Make this more flexible

	if (strstr(name, "spectrum"))
	{
		modelData->setShaderPipeline(ModelData::AlphaDissolveEnvmapped);
		return;
	}

/*
	if (material->sprite())
	{
		modelData->setShaderPipeline(ModelData::Sprite);
		return;
	}
*/
	if ((m_vertexFormat & Position) && (m_vertexFormat & Texcoord))
	{
		modelData->setShaderPipeline(ModelData::FloatTexturedNonShaded);
		return;
	}

	if ((m_vertexFormat & PositionInt) && (m_vertexFormat & Texcoord))
	{
		modelData->setShaderPipeline(ModelData::IntTexturedNonShaded);
		return;
	}

	if ((m_vertexFormat & Position) && !(m_vertexFormat & Texcoord))
	{
		modelData->setShaderPipeline(ModelData::FloatFlatNonShaded);
		return;
	}

	if ((m_vertexFormat & PositionInt) && !(m_vertexFormat & Texcoord))
	{
		modelData->setShaderPipeline(ModelData::IntFlatNonShaded);
		return;
	}

	modelData->setShaderPipeline(ModelData::Dynamic);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

