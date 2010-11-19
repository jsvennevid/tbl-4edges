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

#ifndef zenic_ExporterBackend_h
#define zenic_ExporterBackend_h

#pragma warning (disable: 4511 4530)
#include <map>
#include <list>
#pragma warning (default: 4511 4530)

#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Graphics/Scene/AnimationController.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSLXSIMesh;
class CSLCamera;
class CSLModel;
class CSLEnvelope;
class CSLXSIMaterial;
class CSLImage;
class CSIBCPixMap;
class CSLBaseMaterial;
class CSLXSITransform;
class CSLTemplate;

namespace XSI
{
	class Primitive;
}

namespace zenic
{
	class ModelData;
	class Camera;
	class Node;
	class Model;
	class Material;
	class Serializable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ExporterBackend
{
public:

	ExporterBackend();
	virtual ~ExporterBackend();

	// dotXSI building

	virtual ModelData* buildModelData(CSLModel* mesh, Model* modelNode) = 0;

	virtual Model* buildModel(CSLModel* mesh);
	virtual Node* buildNode(CSLModel* model, Node* node = 0);
	virtual Node* buildCamera(CSLModel* model, Node* node = 0);
	virtual Node* buildGlenz(CSLModel* model, Node* node = 0);
	virtual Node* buildBone(CSLModel* model, Node* node = 0);
	
	virtual Serializable* buildTexture(CSLImage* image, CSIBCPixMap& pixMap);

	// realtime update inside XSI
	
	virtual bool updateModel(Model* model, XSI::Primitive& primitive);

	virtual zenic::Material* buildMaterial(CSLXSIMaterial* material);

	void buildModels();

	// Helper functions

	Node* findNode(CSLModel* model);
	Serializable* findTexture(CSLImage* image);
	zenic::Material* findMaterial(CSLBaseMaterial* material);

	void addMapNode(CSLModel* model, Node* node);
	void addModel(CSLModel* model);
	void addTexture(CSLImage* image, Serializable* texture);
	void addMaterial(CSLXSIMaterial* xsiMaterial, zenic::Material* material);

protected:

	static Matrix4 getMatrix(CSLModel* model);
	static void fillAnimaitonTrack(AnimationController::Track& track, CSLTemplate* xsiTransform,
								   const char* channelName, float timeStart, float frameScale, float keyScale = 1.0f);
	
	std::map<CSLModel*, zenic::Node*> m_nodeRemap;
	std::map<CSLImage*, zenic::Serializable*> m_textureRemap;
	std::map<CSLXSIMaterial*, zenic::Material*> m_materialRemap;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ExporterBackend::addMapNode(CSLModel* model, Node* node)
{
	m_nodeRemap.insert(std::make_pair<CSLModel*, zenic::Node*>(model, node));	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ExporterBackend::addTexture(CSLImage* image, Serializable* texture)
{
	m_textureRemap.insert(std::make_pair<CSLImage*, zenic::Serializable*>(image, texture));	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ExporterBackend::addMaterial(CSLXSIMaterial* xsiMaterial, zenic::Material* material)
{
	m_materialRemap.insert(std::make_pair<CSLXSIMaterial*, zenic::Material*>(xsiMaterial, material));	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Node* ExporterBackend::findNode(CSLModel* model)
{
	std::map<CSLModel*, zenic::Node*>::iterator i;

	i = m_nodeRemap.find(model);

	if (i == m_nodeRemap.end())
		return 0;

	return i->second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Serializable* ExporterBackend::findTexture(CSLImage* image)
{
	std::map<CSLImage*, zenic::Serializable*>::iterator i;

	i = m_textureRemap.find(image);

	if (i == m_textureRemap.end())
		return 0;

	return i->second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline zenic::Material* ExporterBackend::findMaterial(CSLBaseMaterial* material)
{
	std::map<CSLXSIMaterial*, zenic::Material*>::iterator i;

	i = m_materialRemap.find(reinterpret_cast<CSLXSIMaterial*>(material));

	if (i == m_materialRemap.end())
		return 0;

	return i->second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
