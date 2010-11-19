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

#include "DebugRenderer.h"
#include <windows.h>
#include <gl/gl.h>
#include <Shared/Base/Math/Vector3.h>
#include <Shared/Base/Math/Color32.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::text(const char* /*dispText*/, float /*x*/, float /*y*/, const Color32& /*color*/)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color32& color)
{
	glBegin(GL_TRIANGLES);
	glColor3f(color.r * 1.0f / 255.0f, color.g * 1.0f / 255.0f, color.b * 1.0f / 255.0f);
	glVertex3f(p0.x, p0.y, p0.z); 
	glVertex3f(p1.x, p1.y, p1.z); 
	glVertex3f(p2.x, p2.y, p2.z); 
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::line(const Vector3& p1, const Vector3& p2, const Color32& color)
{
	glBegin(GL_LINES);
	glColor3f(color.r * 1.0f / 255.0f, color.g * 1.0f / 255.0f, color.b * 1.0f / 255.0f);
	glVertex3f(p1.x, p1.y, p1.z); 
	glVertex3f(p2.x, p2.y, p2.z); 
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::begin()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugRenderer::end()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
