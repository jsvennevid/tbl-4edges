///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

