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

#include "DotXsiLoader.h"
#include "../Pipeline/ExporterBackend.h"
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Serialize/OutSerializer.h>
//#include <Shared/Graphics/Material.h>
#include <Shared/Geometry/ModelData.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Renderer/Material.h>

// as XSI happens to use deprecated functions and got warnings we turn them off here

#pragma warning(disable : 4996 4244)  

#include <Scene.h>
#include <Model.h>
#include <Mesh.h>
#include <Ikjoint.h>
#include <TriangleList.h>
#include <MaterialLibrary.h>
#include <XsiMaterial.h>
#include <XSIImage.h>
#include <XSIImageData.h>
#include <XSIImageLibrary.h>
#include <SIILJPGFileDriver.h>
#include <SIILPPMFileDriver.h>
#include <SIILPICFileDriver.h>
#include <SIILTGAFileDriver.h>
#include <SIBCPixMap.h> 

#pragma warning(default : 4996 4244)  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DotXsiLoader::DotXsiLoader() : m_serializer(0)
{
	CSIBCPixMap::AddDriver(CSIILJPGFileDriver::Driver());
	CSIBCPixMap::AddDriver(CSIILPPMFileDriver::Driver());
	CSIBCPixMap::AddDriver(CSIILPICFileDriver::Driver());
	CSIBCPixMap::AddDriver(CSIILTGAFileDriver::Driver());

	m_scene = zenic_new CSLScene;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DotXsiLoader::~DotXsiLoader()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DotXsiLoader::warningCallBack(char* string, int value)
{
	ZENIC_INFO("dotXSI Warning: " << string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DotXsiLoader::load(const char* filename)
{
	m_scene->SetWarningCallback(warningCallBack);

	eSI_Error error = (eSI_Error)m_scene->Open((char*)(filename));

	if (error)
	{
		ZENIC_ERROR("Unable to open file " << filename);
		return false;
	}

	if (SI_SUCCESS != m_scene->Read())
	{
		ZENIC_ERROR("Unable to read file  " << filename << endl);
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DotXsiLoader::process(ExporterBackend& backend, OutSerializer& serializer, bool oneObject)
{
	m_serializer = &serializer;
	ZENIC_ASSERT(m_serializer);

	buildImages(backend);
	buildMaterials(backend);	

	SceneInfo* sceneInfo = zenic_new SceneInfo;
	sceneInfo->setLength(10.0f);	// TODO: Fix this length

	if (!oneObject)
		m_serializer->add(sceneInfo);

	traverseRecursive(m_scene->Root(), backend, *sceneInfo, 0, oneObject);	

	for (std::vector<CSLModel*>::iterator i = m_meshList.begin(); i != m_meshList.end(); ++i)
	{
		CSLModel* model = (*i);
		Node* parentNode = 0;

		if (model->Parent())
			parentNode = backend.findNode(model->Parent());

		Node* node = backend.buildModel(*i); 

		if (parentNode && node && !oneObject)
			parentNode->attachChild(node);

		// If we only want to export one object just add the first we find and exit the loop

		if (oneObject) 
		{
			m_serializer->add(node);
			break;
		}
	}

	// 

	saveMaterialAndImages();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* DotXsiLoader::traverseRecursive(CSLModel* child, ExporterBackend& backend, SceneInfo& sceneInfo, int level, bool oneObject)
{
	Node* node = 0;

	char* name = child->GetName();

	for (int i = 0; i < level; ++i)
		zenic::logOut << " ";

	ZENIC_INFO("Name " << child->Name().GetText());

	CSLTemplate::ETemplateType type = child->GetPrimitiveType();

	switch (child->GetPrimitiveType())
	{
		case CSLTemplate::XSI_MESH : 
		{
			addModel(child);
			break;
		}

		case CSLTemplate::SI_NULL_OBJECT :
		{
			// TODO: Do this in a proper way

			if (strstr(child->GetName(), "Glenz"))
			{
				node = backend.buildGlenz(child);
			}
			else if (strstr(child->GetName(), "Camera"))
			{
				node = backend.buildCamera(child);
				sceneInfo.setPrimaryCamera(reinterpret_cast<Camera*>(node));
			}
			else
			{
				node = backend.buildNode(child);
				
				if (!sceneInfo.rootNode())
					sceneInfo.setRootNode(node);
			}

			break;
		}
			
		default :
		{
			node = backend.buildNode(child);
			
			if (!sceneInfo.rootNode())
				sceneInfo.setRootNode(node);
		
			break;
		}
	}

	if (node && !oneObject)
		m_serializer->add(node);

	backend.addMapNode(child, node);

	CSLModel** childList = child->GetChildrenList();

	if (childList)
	{
		for (int i = 0; i < child->GetChildrenCount(); ++i)
		{
			Node* recNode = traverseRecursive(childList[i], backend, sceneInfo, level+1, oneObject);
			
			if (node && recNode)
				node->attachChild(recNode);
		}
	}


	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DotXsiLoader::buildImages(ExporterBackend& backend)
{
	CSLImageLibrary* imageLibrary = m_scene->GetImageLibrary();

	if (!imageLibrary)
		return;

	CSLImage** imageList = imageLibrary->GetImageList();
	int imageCount = imageLibrary->GetImageCount();

	m_textures.clear();

	for (int i = 0; i < imageCount; ++i)
	{
		CSIBCPixMap pixMap;

		CSLImage* image = imageList[i];
		ZENIC_ASSERT(image);

		char* fileName = image->GetSourceFile();

		if (SI_SUCCESS != CSIBCPixMap::Load(fileName, pixMap))
		{
			ZENIC_WARNING("Unable to load file: " << fileName << " not supported format or missing.");
			continue;
		}

		Serializable* texture = backend.buildTexture(image, pixMap);

		if (texture)
		{
			backend.addTexture(image, texture);
			m_textures.push_back(texture);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DotXsiLoader::saveMaterialAndImages()
{
	for (std::vector<Material*>::iterator i = m_materials.begin(); i != m_materials.end(); ++i)
	{
		m_serializer->add(*i);
		(*i)->addInternal(*m_serializer);	// fix me
	}

	//for (std::vector<Serializable*>::iterator i = m_textures.begin(); i != m_textures.end(); ++i)
	//	m_serializer->add(*i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DotXsiLoader::buildMaterials(ExporterBackend& backend)
{
	CSLMaterialLibrary* library = m_scene->GetMaterialLibrary();
	uint materialCount = library->GetMaterialCount();
	
	CSLBaseMaterial** materialList = library->GetMaterialList();
	
	for (uint i = 0; i < materialCount; ++i)
	{
		if (materialList[i]->Type() != CSLTemplate::XSI_MATERIAL)
			continue;

		CSLXSIMaterial* xsiMaterial = static_cast<CSLXSIMaterial*>(materialList[i]);

		Material* material = backend.buildMaterial(xsiMaterial);
		
		if (material)
		{
			backend.addMaterial(xsiMaterial, material);
			m_materials.push_back(material);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
