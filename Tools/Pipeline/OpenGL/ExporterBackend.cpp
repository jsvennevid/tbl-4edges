///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ExporterBackend.h"
#include "../TriangleStrip.h"
#include "../DegenerateMesh.h"
#include "ShaderBuilder.h"

#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Storage/String.h>
#include <Shared/Graphics/Scene/BoneArray.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Geometry/OpenGL/ModelData.h>
#include <Shared/Geometry/OpenGL/Texture.h>
#include <Shared/Geometry/OpenGL/TextureData.h>

#include <Shared/Graphics/Renderer/OpenGL/Material.h>

#pragma warning(disable : 4505 4100 4996 4244 4511)

#include <windows.h>
#include <XSIShape.h>
#include <XSIMesh.h>
#include <Model.h>
#include <TriangleList.h>
#include <Envelope.h>
#include <IkJoint.h>
#include <Transform.h>
#include <XSIMaterialInfo.h>
#include <XSIMaterial.h>
#include <XSIImage.h>
#include <XSIImageData.h>
#include <ConnectionPoint.h>
#include <XSIShader.h>
#include <ShaderConnectionPoint.h>
#include <CustomPset.h>
#include <VariantParameter.h>
#include <ShaderInstanceData.h>
#include <SIBCPixMap.h> 
#include <XSITriangleList.h>
#include <XSIShader.h>
#include <GlobalMaterial.h>

#pragma warning(default : 4505 4100 4996 4244 4511)  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ExporterBackend::SkinInfo* ExporterBackend::buildSkinInfo(CSLModel* model, Model* modelNode, u32* /*remapTable*/)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

zenic::ModelData* ExporterBackend::buildModelData(CSLModel* model, Model* modelNode)
{
	ModelData* target = zenic_new ModelData;

	CSLXSIMesh* mesh = static_cast<CSLXSIMesh*>(model->Primitive());
	CSLXSIShape* shape = mesh->XSIShape();

	DegenerateMesh degMesh;

	degMesh.process(mesh);

	TriangleStrip triangleStripper(TriangleStrip::Actc);

	const TriangleStrip::StripInfo& stripInfo = triangleStripper.process(&degMesh);

	if (stripInfo.remapTable == 0)
	{
		ZENIC_ERROR("Failed to trianglestrip " << mesh->GetName() << " mesh will be skipped");
		zenic_delete target;
		return 0;
	}

	// Set the correct material to the modelData

	if (model->GlobalMaterial())
		target->setMaterial((opengl::Material*)findMaterial(model->GlobalMaterial()->GetMaterial()));

	SkinInfo* skinInfo = 0;

	if (model->GetEnvelopeCount() != 0)
	{
		skinInfo = buildSkinInfo(model, modelNode, stripInfo.remapTable);
		target->setType(ModelData::Skinned);
	}

	static float colors[] = 
	{
		0,  1.0f, 1.0f, 0,
		1.0f,  0, 1.0f, 0,
		0,  1.0f,    0, 0,
		1.0f, 1.0f,  0, 0
	};

	target->polygons().allocate(u32(stripInfo.stripList.size()));
	ModelData::PolygonList* polygonLists = target->polygons().objects();

	const std::vector<DegenerateMesh::Vertex>& vertices = degMesh.vertexList();

	uint d = 0;

	for (std::vector<DataPtr<u32>*>::const_iterator i = stripInfo.stripList.begin(); i != stripInfo.stripList.end(); ++i, ++d)
	{
		u32 colorSelection = (d & 3) << 2;

		DataPtr<u32>* triStripList = (*i);
	
		// process strip		

		u32* stripList = triStripList->objects();
		u32 stripCount = triStripList->count();

		ModelData::PolygonList& polygonList = polygonLists[d];

		polygonList.setPolygonType(ModelData::TriStrips);

		if (skinInfo)
			polygonList.setVertexFormat(ModelData::Postion | ModelData::Color | ModelData::Weight);
		else
			polygonList.setVertexFormat(ModelData::Postion | ModelData::Color | ModelData::Uv1);

		if (skinInfo)
			polygonList.vertexStream().allocate((stripCount * 14));	// coords and colors
		else
			polygonList.vertexStream().allocate((stripCount * 8));	// 3 coords, 3 colors, 2 uv 

		f32* tempStream = polygonList.vertexStream().objects();
				
		for (uint d = 0; d < stripCount; ++d)
		{
			const DegenerateMesh::Vertex& vertex = vertices[stripList[d]];

			*tempStream++ = vertex.position.x;			
			*tempStream++ = vertex.position.y;			
			*tempStream++ = vertex.position.z;			
	
			*tempStream++ = colors[colorSelection + 0];			
			*tempStream++ = colors[colorSelection + 1];			
			*tempStream++ = colors[colorSelection + 2];			

			*tempStream++ = vertex.uv.x;			
			*tempStream++ = vertex.uv.y;			

			if (skinInfo)
			{
				const SkinInfo& currInfo = skinInfo[stripList[d]];

				for (uint d = 0; d < 4; ++d)
				{
					*tempStream++ = float(currInfo.indices[d]);
					*tempStream++ = float(currInfo.weights[d]);
				}
			}
		}
	}

	delete [] skinInfo;

	return target;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Serializable* ExporterBackend::buildTexture(CSLImage* image, CSIBCPixMap& pixMap)
{
	uint depth = pixMap.GetTotalPixelDepthByte();

	// We only handly 32 bit and 24bit textures right now

	if (depth != 4 && depth != 3)
	{
		ZENIC_WARNING("Unsupported pixeldepth (" << pixMap.GetTotalPixelDepthByte() << ") for texture " << image->GetSourceFile() <<
					 "skipping");
		return 0;
	}

	Texture* texture = zenic_new Texture;
	TextureData* textureData = zenic_new TextureData;

	textureData->setWidth(u16(pixMap.GetWidth()));
	textureData->setHeight(u16(pixMap.GetHeight()));
	textureData->setFlags(TextureData::RGB);

	DataPtr<u8> imageData;

	uint imageSize = pixMap.GetWidth() * pixMap.GetHeight() * depth;

	imageData.allocate(imageSize);
	memcpy(imageData.objects(), pixMap.GetMap(), imageSize);

	textureData->setImageData(imageData);
	texture->setData(textureData);

	return texture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

zenic::Material* ExporterBackend::buildMaterial(CSLXSIMaterial* xsiMaterial)
{
	opengl::Material* material = zenic_new opengl::Material;
		
	ShaderBuilder::build(material, *this, xsiMaterial);


	/*
	uint connectionCount = material->GetConnectionPointCount();
	CSLConnectionPoint** connectionPoints = material->GetConnectionPointList();

	for (uint i = 0; i < connectionCount; ++i)
	{
		CSLConnectionPoint* connectionPoint = connectionPoints[i];
		CSLXSIShader* shader = connectionPoint->GetShader();

		if (shader)
		{
			if (shader->GetName())
				ZENIC_INFO("material connection " << shader->GetName());
		}
	}

	uint shaderCount = material->GetShaderCount();
	CSLXSIShader** shaders = material->GetShaderList();

	for (uint i = 0; i < shaderCount; ++i)
	{
		CSLXSIShader* shader = shaders[i];

		if (shader->GetName())
			ZENIC_INFO("Shader connection " << shader->GetName());

		uint subCount = shader->GetConnectionPointCount();
		CSLShaderConnectionPoint** shaderConnectionPoints = shader->GetConnectionPointList();

		for (uint k = 0; k < subCount; ++k)
		{
			CSLShaderConnectionPoint* shaderConnectionPoint = shaderConnectionPoints[k];
			CSLShaderConnectionPoint::EConnectionType shaderType = shaderConnectionPoint->GetConnectionType();
	
			ZENIC_INFO("  Name " << shaderConnectionPoint->GetName() << " " << shaderType);

			if (shaderType == CSLShaderConnectionPoint::SI_IMAGE)
			{
				const char* name = shaderConnectionPoint->GetImage();
				ZENIC_INFO("  Image " << name);
			}
			else
			{
				CSLXSIShader* subShader = shaderConnectionPoint->GetShader();
		
				if (subShader)
				{
					if (subShadeand r->GetName())
						ZENIC_INFO("  Shader " << subShader->GetName());
					else
						ZENIC_INFO("  Shader N/A");
				}
			}
		}

		ZENIC_INFO("  ======================================================================");

		CSLCustomPSet** customPsetList = shader->GetCustomPSetList();
		subCount = shader->GetCustomPSetCount();

		for (uint k = 0; k < subCount; ++k)
		{
			CSLCustomPSet* customPset = customPsetList[k];
			uint parameterCount = customPset->GetParameterCount();

			ZENIC_INFO("  Name " << customPset->GetName() << " Type " << customPset->GetPropagationType() <<
				      " Parms " << parameterCount);

			CSLVariantParameter** parameterList = customPset->GetParameterList();

			for (uint j = 0; j < parameterCount; ++j)
			{
				CSLVariantParameter* parameter = parameterList[j];
				ZENIC_INFO("    Name, Value " << parameter->GetName());
			}
		}

		CSLShaderInstanceData** instanceDataList = shader->GetInstanceDataList();
		subCount = shader->GetInstanceDataCount();

		for (uint k = 0; k < subCount; ++k)
		{
			CSLShaderInstanceData* instanceData = instanceDataList[k];
			CSLVariantParameter** parameterList = instanceData->GetParameterList();

			for (int j = 0; j < instanceData->GetParameterCount(); ++j)
			{
				CSLVariantParameter* parameter = parameterList[j];
				ZENIC_INFO("    Name, Value " << parameter->GetName());
			}
		}

		CSLVariantParameter** parameterList = shader->GetParameterList();

		for (int j = 0; j < shader->GetParameterCount(); ++j)
		{
			CSLVariantParameter* parameter = parameterList[j];

			if (parameter)
				ZENIC_INFO("    Name, Value " << parameter->GetName() << " " << parameter->GetFloatValue());
		}

	}
*/
	return material;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ExporterBackend::traverseShaders(CSLShader* /*shader*/, uint /*level*/)
{
	//if (shader->GetName())
	//	ZENIC_INFO("Shader connection " << shader->GetName());


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
