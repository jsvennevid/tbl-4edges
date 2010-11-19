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

#include "SceneInfo.h"

#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Io/Log.h>

#include <Shared/Graphics/Scene/Camera.h>
#include <Shared/Graphics/Scene/Node.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(SceneInfo, SERIALIZABLE_ID('Z','N','I','C'), SERIALIZABLE_ID('S','N','F','O'))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SceneInfo::SceneInfo() :
	m_primaryCamera(0),
	m_rootNode(0),
	m_length(0.0f)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SceneInfo::serialize(Serializer& s)
{
	SerializableVersion version(1, factory());

	s << version;

	s.descriptor("m_primaryCamera") << m_primaryCamera;
	s.descriptor("m_rootNode") << m_rootNode;
	s.descriptor("m_length") << m_length;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SceneInfo::switchToCamera(uint cameraId)
{
	ZENIC_ASSERT(cameraId < m_cameras.count());
	Camera* camera = static_cast<Camera*>(m_cameras.buffer()[cameraId]);
	setPrimaryCamera(camera);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

