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

#ifndef zenic_ps2_PadCamera
#define zenic_ps2_PadCamera

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Input/Ps2/Pad.h>
#include <Shared/Graphics/Scene/Camera.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Vector3;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PadCamera
{
public:

	PadCamera(Pad& pad);
	PadCamera(Pad& pad, const Vector3& position);

	void update();
	const zenic::Camera& sceneCamera() const;
	void setSceneCamera(const zenic::Camera& sceneCamera);

private:

	zenic::Camera m_sceneCamera;
	Pad& m_pad;
	float m_rotX;
	float m_rotY;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const zenic::Camera& PadCamera::sceneCamera() const
{
	return m_sceneCamera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void PadCamera::setSceneCamera(const zenic::Camera& sceneCamera)
{
	m_sceneCamera = sceneCamera;
}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}


#endif
