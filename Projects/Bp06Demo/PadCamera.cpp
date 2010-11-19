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

