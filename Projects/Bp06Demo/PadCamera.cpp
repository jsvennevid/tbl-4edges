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

#include "PadCamera.h"
#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Base/Io/Log.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PadCamera::PadCamera(Pad& pad) :
	m_pad(pad),
	m_rotX(0.0f), 
	m_rotY(0.0f)
{
	Matrix4 matrix;
	matrix = matrix.identity();
	m_sceneCamera.setTransform(matrix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PadCamera::PadCamera(Pad& pad, const Vector3& pos) :
	m_pad(pad),
	m_rotX(0.0f),
	m_rotY(0.0f)
{
	Matrix4 matrix;

	matrix = matrix.identity();
	matrix.setPosition(pos);

	m_sceneCamera.setTransform(matrix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PadCamera::update()
{
	Matrix4 rotH;
	Matrix4 rotP;

	Matrix4 cameraMatrix = m_sceneCamera.transform();
	Vector3 pos = cameraMatrix.position();

	float deltaTime = 0.01f;

	m_rotX += m_pad.rstickV() * deltaTime * 2.0f;
	m_rotY -= m_pad.rstickH() * deltaTime * 2.0f;

	rotH = rotH.makeXrotation(m_rotX);
	rotP = rotP.makeYrotation(m_rotY);

	cameraMatrix = rotH * rotP;

	pos = pos + (cameraMatrix.x() * m_pad.lstickH() + 
				 cameraMatrix.z() * m_pad.lstickV()) * (deltaTime * 100.0f);

	cameraMatrix.setPosition(pos);
	m_sceneCamera.setTransform(cameraMatrix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

