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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DebugRenderer.h"
#include <Shared/Base/Storage/String.h>
#include <Shared/Base/Math/Vector3.h>
#include <Shared/Base/Math/Matrix4.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 DebugRenderer::m_cubeCoords[] =
{
	Vector3(-1.0f,  1.0f,  1.0f),
	Vector3( 1.0f,  1.0f,  1.0f),
	Vector3( 1.0f, -1.0f,  1.0f),
	Vector3(-1.0f, -1.0f,  1.0f),
	Vector3(-1.0f,  1.0f, -1.0f),
	Vector3( 1.0f,  1.0f, -1.0f),
	Vector3( 1.0f, -1.0f, -1.0f),
	Vector3(-1.0f, -1.0f, -1.0f)
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::text(const zenic::String& dispText, float x, float y, const Color32& color)
{
	text(static_cast<const char*>(dispText.str()), x, y, color); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::quad(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, 
						 const Color32& color)
{
	triangle(p0, p1, p2, color);
	triangle(p0, p2, p3, color);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::thickLine(const Vector3& /*p0*/, const Vector3& /*p2*/, float /*thickness*/, const Color32& /*color*/)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::cube(const Vector3& pos, const Matrix4& rot, float sideLength, const Color32& color)
{
	Vector3 coords[8];

	for (uint i = 0; i < 8; ++i)
		coords[i] = rot.apply((m_cubeCoords[i] * sideLength)) + pos; 

	quad(coords[1], coords[0], coords[3], coords[2], Color32(255, 0, 0, 0));
	quad(coords[4], coords[5], coords[6], coords[7], Color32(0, 255, 0, 0));
	quad(coords[0], coords[1], coords[5], coords[4], Color32(0, 0, 255, 0));
	quad(coords[7], coords[6], coords[2], coords[3], Color32(255, 255, 0, 0));
	quad(coords[5], coords[1], coords[2], coords[6], Color32(255, 0, 255, 0));
	quad(coords[0], coords[4], coords[7], coords[3], color);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::axisCross(const Vector3& pos, float size)
{
	line(pos, Vector3(pos.x + size, pos.y, pos.z), Color32(255, 0, 0, 0)); 
	line(pos, Vector3(pos.x, pos.y + size, pos.z), Color32(0, 255, 0, 0)); 
	line(pos, Vector3(pos.x, pos.y, pos.z + size), Color32(0, 0, 255, 0)); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::box(const Vector3& /*pos*/, const Matrix4& /*rot*/, const Vector3& /*sideLength*/, const Color32& /*color*/)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::sphere(const Vector3& /*pos*/, const Matrix4& /*rot*/, float /*radius*/, const Color32& /*color*/)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::sphere(const Vector3& /*pos*/, float /*radius*/, const Color32& /*color*/)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::circle(const Vector3& /*pos*/, const Vector3& /*plane*/, float /*radius*/, float /*thickness*/, const Color32& /*color*/)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
