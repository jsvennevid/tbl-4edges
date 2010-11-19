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

#pragma warning(disable : 4505)  

#include "ExporterBackend.h"

// as XSI happens to use deprecated functions and got warnings we turn them off here

#pragma warning(disable : 4100 4996 4244)  

#include <windows.h>
#include <Scene.h>
#include <Model.h>
#include <Mesh.h>
#include <Transform.h>
#include <TriangleList.h>
#include <XsiMaterial.h>
#include <XSITransform.h>
#include <FCurve.h>
#include <BaseFCurve.h>
#include <SceneInfo.h>

#pragma warning(default : 4100 4996 4244)  

#include <Shared/Geometry/ModelData.h>

#include <Shared/Graphics/Scene/Node.h>
#include <Shared/Graphics/Scene/GlenzNode.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/AnimationController.h>
#include <Shared/Graphics/Scene/Camera.h>
#include <Shared/Base/Io/Log.h>

#include <Shared/Base/Math/Matrix4.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ExporterBackend::ExporterBackend()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ExporterBackend::~ExporterBackend()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static CSLFCurve* findCurve(CSLFCurve** curves, int curveCount, const char* name)
{
	for (int i = 0; i < curveCount; ++i)
	{
		const char* fcurveName = curves[i]->GetFCurveTypeAsString();

		if (!strcmp(name, fcurveName))
			return curves[i];
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ExporterBackend::fillAnimaitonTrack(AnimationController::Track& track, CSLTemplate* xsiTemplate, 
										 const char* channelName, float timeStart, float frameScale, float keyScale)
{
	CSLFCurve** curves = xsiTemplate->FCurves();
	const int curveCount = xsiTemplate->GetFCurveCount();

	CSLFCurve* curve = findCurve(curves, curveCount, channelName);	

	if (curve)
	{
		const CSLLinearKey* keys = curve->GetLinearKeyListPtr();

		ZENIC_ASSERT_DESC(keys, "Only linear curves supported. Please re-export with Linear curves selected.");

		long keyCount = curve->GetKeyCount();

		// find lowest and highest value

		float maxValue = -100000000.0f;
		float minValue = 100000000.0f;

		float maxTime = -100000000.0f;
		float minTime = 100000000.0f;

		for (long keyIter = 0; keyIter < keyCount; ++keyIter)
		{
			float value = keys[keyIter].m_fValue * keyScale;
			float time = (keys[keyIter].m_fTime - timeStart) * frameScale;

			if (value < minValue) 
				minValue = value;

			if (value > maxValue)
				maxValue = value;

			if (time < minTime)
				minTime = time;

			if (time > maxTime)
				maxTime = time;
		}

		float totalMaxValue = fabs(maxValue - minValue);
		float scaleValue = 65530.0f / totalMaxValue;
		float invScaleValue = 1.0f / scaleValue;

		float totalMaxTime = fabs(maxTime - minTime);
		float scaleTime = 65530.0f / totalMaxTime;
		float invScaleTime = 1.0f / scaleTime;

		track.compression = AnimationController::Track::Compressed16bit;
		track.data.resize((8 + 8) + (keyCount * 4)); // 16 (scale/offset * 2) keys * 4 (time, key)	
		u8* keyData = track.data.objects();

		// Set scale and offset

		float* floatData = (float*)keyData;
		floatData[0] = invScaleValue;
		floatData[1] = minValue;
		floatData[2] = invScaleTime;
		floatData[3] = minTime;

		u16* shortData = (u16*)(keyData + 16);

		for (long keyIter = 0; keyIter < keyCount; ++keyIter)
		{
			float value = (keys[keyIter].m_fValue * keyScale) - minValue;
			float time = ((keys[keyIter].m_fTime - timeStart) * frameScale) - minTime;
			shortData[0] = (u16)(time * scaleTime);
			shortData[1] = (u16)(value * scaleValue);
			shortData += 2;
		}
	}
	else
	{
		// Set static value here
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* ExporterBackend::buildNode(CSLModel* model, Node* node)
{
	if (!node)
		node = zenic_new Node;

	node->setTransform(getMatrix(model));

	// TODO: Platform specific animation data

	if (!model->XSITransform())
		return node;

	CSLXSITransform* transform = model->XSITransform();

	if (transform->GetFCurveCount() == 0)
		return node;

	// TODO: Get this once only?
    // Timing conversions from XSI frames to Zenic time in seconds

	const float invFrameRate = 1.0f / model->Scene()->SceneInfo()->GetFrameRate();
	const float startTime = model->Scene()->SceneInfo()->GetStart(); 

	// Hardcoded 9 tracks for now.

	AnimationController* controller = zenic_new AnimationController;

	controller->setTrackCount(9);

	fillAnimaitonTrack(controller->track(AnimationController::PosX), transform, "posx", startTime, invFrameRate);
	fillAnimaitonTrack(controller->track(AnimationController::PosY), transform, "posy", startTime, invFrameRate);
	fillAnimaitonTrack(controller->track(AnimationController::PosZ), transform, "posz", startTime, invFrameRate);

	fillAnimaitonTrack(controller->track(AnimationController::RotateX), transform, "rotx", startTime, invFrameRate, 3.1415f / 180.0f);
	fillAnimaitonTrack(controller->track(AnimationController::RotateY), transform, "roty", startTime, invFrameRate, 3.1415f / 180.0f);
	fillAnimaitonTrack(controller->track(AnimationController::RotateZ), transform, "rotz", startTime, invFrameRate, 3.1415f / 180.0f);

	fillAnimaitonTrack(controller->track(AnimationController::ScaleX), transform, "sclx", startTime, invFrameRate);
	fillAnimaitonTrack(controller->track(AnimationController::ScaleY), transform, "scly", startTime, invFrameRate);
	fillAnimaitonTrack(controller->track(AnimationController::ScaleZ), transform, "sclz", startTime, invFrameRate);

	// TODO: Handle a list of controllers

	node->setControllers(controller);

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Model* ExporterBackend::buildModel(CSLModel* model)
{
	Model* modelNode = zenic_new Model;

	buildNode(model, modelNode);

	modelNode->setModelData(buildModelData(model, modelNode));

	return modelNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* ExporterBackend::buildBone(CSLModel* /*model*/, Node* /*node*/)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ExporterBackend::updateModel(Model* model, XSI::Primitive& primitive)
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* ExporterBackend::buildGlenz(CSLModel* model, Node* node)
{
	if (!node)
		node = zenic_new GlenzNode;

	node->setTransform(getMatrix(model));

	return node;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* ExporterBackend::buildCamera(CSLModel* model, Node* node)
{
	if (!node)
		node = zenic_new Camera;

	node->setTransform(getMatrix(model));

	// TODO: Platform specific animation data

	if (!model->XSITransform())
		return node;

	CSLXSITransform* xsiTemplate = model->XSITransform();

	if (xsiTemplate->GetFCurveCount() == 0)
		return node;

	// TODO: Get this once only?
	// Timing conversions from XSI frames to Zenic time in seconds

	const float invFrameRate = 1.0f / model->Scene()->SceneInfo()->GetFrameRate();
	const float startTime = model->Scene()->SceneInfo()->GetStart(); 

	// Hardcoded 9 tracks for now.

	AnimationController* controller = zenic_new AnimationController;

	controller->setTrackCount(6);

	fillAnimaitonTrack(controller->track(AnimationController::PosX), xsiTemplate, "posx", startTime, invFrameRate);
	fillAnimaitonTrack(controller->track(AnimationController::PosY), xsiTemplate, "posy", startTime, invFrameRate);
	fillAnimaitonTrack(controller->track(AnimationController::PosZ), xsiTemplate, "posz", startTime, invFrameRate);

	fillAnimaitonTrack(controller->track(AnimationController::RotateX), xsiTemplate, "rotx", startTime, invFrameRate, 3.1415f / 180.0f);
	fillAnimaitonTrack(controller->track(AnimationController::RotateY), xsiTemplate, "roty", startTime, invFrameRate, 3.1415f / 180.0f);
	fillAnimaitonTrack(controller->track(AnimationController::RotateZ), xsiTemplate, "rotz", startTime, invFrameRate, 3.1415f / 180.0f);

	// TODO: Handle a list of controllers

	node->setControllers(controller);

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Serializable* ExporterBackend::buildTexture(CSLImage* /*image*/, CSIBCPixMap& /*pixMap*/) 
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Material* ExporterBackend::buildMaterial(CSLXSIMaterial* material)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Matrix4 ExporterBackend::getMatrix(CSLModel* model)
{
	Matrix4 transform;

	// Get transform related data

	if (model->XSITransform() != 0)
	{
		const CSIBCVector3D& rotation = model->XSITransform()->GetEulerRotation();
		const CSIBCVector3D& pos = model->XSITransform()->GetTranslation();

		Matrix4 mx;
		Matrix4 my;
		Matrix4 mz;

		mx.makeXrotation(rotation.GetX() * 3.1415f / 180.0f);
		my.makeYrotation(rotation.GetY() * 3.1415f / 180.0f);
		mz.makeZrotation(rotation.GetZ() * 3.1415f / 180.0f);

		transform = mx * my * mz;
		transform.setPosition(Vector3(pos.GetX(), pos.GetY(), pos.GetZ()));

		// This is odd, shoulnt need to do this

		ZENIC_INFO(model->GetName() << " " << pos.GetX() << " " << pos.GetY() << " " << pos.GetZ());		
	}
	else
	{
		transform = transform.identity();
	}

	return transform;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#pragma warning(default : 4505 4100 4996 4244)  

