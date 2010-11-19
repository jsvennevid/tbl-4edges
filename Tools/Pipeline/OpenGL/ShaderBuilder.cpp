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

