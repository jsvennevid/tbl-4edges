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

#ifndef zenic_SceneInfo_h
#define zenic_SceneInfo_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Storage/Array.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Camera;
	class Node;
	class Model;

	namespace ps2
	{
		class Texture;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SceneInfo : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	SceneInfo();

	void serialize(Serializer& s);

	float length() const;
	void setLength(float length);

	Camera* primaryCamera() const;
	void setPrimaryCamera(Camera* primaryCamera);

	void switchToCamera(uint cameraId);

	Node* rootNode() const;
	void setRootNode(Node* rootNode);

	void setTextures(const Array<ps2::Texture*>& textures);
	Array<ps2::Texture*>& textures();
	Array<Model*>& models();
	Array<Node*>& cameras();

private:

	Camera* m_primaryCamera;
	Node* m_rootNode;
	float m_length;
	Array<ps2::Texture*> m_textures;	// temporary
	Array<Model*> m_models; // temporary
	Array<Node*> m_cameras;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float SceneInfo::length() const
{
	return m_length;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void SceneInfo::setLength(float length)
{
	m_length = length;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Camera* SceneInfo::primaryCamera() const
{
	return m_primaryCamera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void SceneInfo::setPrimaryCamera(Camera* primaryCamera)
{
	m_primaryCamera = primaryCamera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Node* SceneInfo::rootNode() const
{
	return m_rootNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void SceneInfo::setRootNode(Node* rootNode)
{
	m_rootNode = rootNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void SceneInfo::setTextures(const Array<ps2::Texture*>& textures)
{
	m_textures = textures;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Array<ps2::Texture*>& SceneInfo::textures()
{
	return m_textures;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Array<Model*>& SceneInfo::models()
{
	return m_models;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Array<Node*>& SceneInfo::cameras()
{
	return m_cameras;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif

