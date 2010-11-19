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

#include "ShaderBuilder.h"
#include "ExporterBackend.h"
#include <XSIMaterial.h>
#include <XSIShader.h>
#include <XSIImageLibrary.h>
#include <ConnectionPoint.h>
#include <ShaderConnectionPoint.h>
#include <Scene.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Storage/String.h>
#include <Shared/Graphics/Renderer/OpenGL/Material.h>
#include <Shared/Graphics/Renderer/OpenGL/Shader.h>
#include <Shared/Graphics/Renderer/OpenGL/ShaderNode.h>
#include <Shared/Graphics/Renderer/OpenGL/TextureShaderNode.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ShaderBuilder::build(Material* material, ExporterBackend& backend, CSLXSIMaterial* xsiMaterial)
{
	int shaderCount = xsiMaterial->GetShaderCount();
	CSLXSIShader** shaders = xsiMaterial->GetShaderList();

	ZENIC_INFO("Material name " << xsiMaterial->Name().GetText()); 

	Shader* shader = zenic_new Shader;
	ShaderNode* shaderNode = 0;

	for (int i = shaderCount-1; i >= 0; --i)
	{
		CSLXSIShader* xsiShader = shaders[i];

		String shaderId(xsiShader->GetProgID());

		u32 hash = shaderId.quickHash();

		switch (hash)
		{
			case SoftImageTxt2d : shaderNode = textureShader(material, backend, xsiShader); break;
			default:
			{
				ZENIC_WARNING("Unable to build Shader of type: " << shaders[i]->GetProgID());
				shaderNode = 0;
			}
		}

		if (shaderNode)
			shader->addNode(shaderNode);
	}

	material->addShader(shader);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ShaderNode* ShaderBuilder::textureShader(Material* material, ExporterBackend& backend, CSLXSIShader* xsiShader)
{
	TextureShaderNode* shaderNode = zenic_new TextureShaderNode;

	// Find image pointer

	uint subCount = xsiShader->GetConnectionPointCount();
	CSLShaderConnectionPoint** shaderConnectionPoints = xsiShader->GetConnectionPointList();

	for (uint k = 0; k < subCount; ++k)
	{
		CSLShaderConnectionPoint* shaderConnectionPoint = shaderConnectionPoints[k];
		CSLShaderConnectionPoint::EConnectionType shaderType = shaderConnectionPoint->GetConnectionType();

		if (shaderType == CSLShaderConnectionPoint::SI_IMAGE)
		{
			CSLImage* image = xsiShader->Scene()->GetImageLibrary ()->FindImage(shaderConnectionPoint->GetImage());

			Texture* texture = reinterpret_cast<Texture*>(backend.findTexture(image));
			shaderNode->setTexture(texture);
		}
	}

	return shaderNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

