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

#ifndef zenic_DebugRenderer_h
#define zenic_DebugRenderer_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Debug/Assert.h>
#include <Shared/Base/Math/Color32.h>
#include <Shared/Base/Storage/Singleton.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forwards
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class String;
	class Vector3;
	class Matrix4;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DebugRenderer : public Singleton<DebugRenderer>
{
public:

	// Interface

	virtual void text(const char* dispText, float x, float y, const Color32& color = Color32(255, 255, 255, 0)) = 0;
	virtual void triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color32& color) = 0;
	virtual void line(const Vector3& p1, const Vector3& p2, const Color32& color = Color32(255, 255, 255, 0)) = 0;

	// all rendering must be inside begin/endblock

	virtual void begin() = 0;
	virtual void end() = 0;

	// Drawing functions

	void text(const String& dispText, float x, float y, const Color32& color = Color32(255, 255, 255, 0));

	void quad(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, 
			  const Color32& color = Color32(255, 255, 255, 0));

	void thickLine(const Vector3& p1, const Vector3& p2, float thickness = 0.03f, 
				   const Color32& color = Color32(255, 255, 255, 0));

	void axisCross(const Vector3& pos, float size = 1.0f);

	void cube(const Vector3& x, const Matrix4& rot, float sideLength = 1.0f, 
			  const Color32& color = Color32(255, 255, 255, 0));
	void box(const Vector3& x, const Matrix4& rot, const Vector3& sideLength, 
			 const Color32& color = Color32(255, 255, 255, 0));

	void sphere(const Vector3& x, const Matrix4& rot, float radius = 1.0f, 
				const Color32& color = Color32(255, 255, 255, 0));
	void sphere(const Vector3& x, float radius = 1.0f, const Color32& color = Color32(255, 255, 255, 0));
	void circle(const Vector3& position, const Vector3& plane, float radius, float thickness = 0.03f, 
				const Color32& color = Color32(255, 255, 255, 0));
protected:

	static Vector3 m_cubeCoords[];
};

}


#endif

