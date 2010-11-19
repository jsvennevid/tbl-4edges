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

#ifndef zenic_ps2_Camera_h
#define zenic_ps2_Camera_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Base/Math/Vector4.h>
#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Camera
{
public:

	Camera();

	void addToChain(uint vu1RamPos = 0);
	void update(const float& deltaTime, const float& sceneScale);

	u8* loadFromImage(u8* memory);

	void init();

	void setSize(const u32& w, const u32& h);
	void setOffset(const u32& x, const u32& y);
	void setNearPlane(const float& f);
	void setFarPlane(const float& f);
	void setViewWindowSize(const float& size);
	void setAspectRatio(const float& ratio);
	void setPosition(const Vector4& pos);
	void setCameraTransform(const Matrix4& transform);

	void setMinZ(float z);
	void setMaxZ(float z);

	Vector3 position();

private:

	u32 m_width;
	u32	m_height;

	float m_nearPlane; 
	float m_farPlane;
	float m_viewWindowSize;
	float m_aspectRatio;

	float m_minZ;
	float m_maxZ;

	Matrix4 m_clipMatrix;
	Matrix4 m_viewMatrix;
	Matrix4 m_matrix;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setSize(const u32& w,const u32& h)
{
	m_width = w; 
	m_height = h; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setNearPlane(const float& plane)
{
	m_nearPlane = plane;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setFarPlane(const float& plane)
{
	m_farPlane = plane;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setViewWindowSize(const float& size)
{
	m_viewWindowSize = size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setAspectRatio(const float& ratio)
{
	m_aspectRatio = ratio;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setCameraTransform(const Matrix4& transform)
{
	m_matrix = transform;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setPosition(const Vector4& pos)
{
	float* matrix = m_matrix.matrix();
	matrix[(3<<2)+0] = pos.x;
	matrix[(3<<2)+1] = pos.y;
	matrix[(3<<2)+2] = pos.z;
	matrix[(3<<2)+3] = pos.w;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setMinZ(float z)
{
	m_minZ = z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Camera::setMaxZ(float z)
{
	m_maxZ = z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Vector3 Camera::position()
{
	return m_matrix.position();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}


#endif
