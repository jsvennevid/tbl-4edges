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

#ifndef zenic_Node_h
#define zenic_Node_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Bound.h"

#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Base/Storage/Array.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Controller;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Node : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	enum Type
	{
		NodeType,
		CameraType
	};

	Node();
	virtual ~Node();

	virtual void update(float time);
	virtual void serialize(Serializer& s);

	void collect(Array<Node*>& nodes, Type type);
	virtual bool isType(Type type);

	void setTransform(const Matrix4& transform);
	const Matrix4& transform() const;

	void setWorldTransform(const Matrix4& worldTransform);
	void updateWorldTransform();
	const Matrix4& worldTransform() const;

	void setBound(const Bound& bound);
	void updateBound() const;
	const Bound& bound() const;

	void attachChild(Node* child);
	void insertChild(Node* sibling, Node* child);
	void detachChild(Node* child);

	void setParent(Node* parent);
	Node* parent() const;

	void setFirstChild(Node* firstChild);
	Node* firstChild() const;

	void setLastChild(Node* lastChild);
	Node* lastChild() const;

	void setPrevSibling(Node* prevSibling);
	Node* prevSibling() const;

	void setNextSibling(Node* nextSibling);
	Node* nextSibling() const;

	void updateControllers(float time);

	void setControllers(Controller* controllers);
	Controller* controllers() const;

	static Node* traverse(Node* root, Node* current);

	const Vector3& scale() const;
	void setScale(const Vector3& scale);

private:

	Node* m_parent;
	Node* m_firstChild;
	Node* m_lastChild;
	Node* m_prevSibling;
	Node* m_nextSibling;

	Matrix4 m_transform;
	Matrix4 m_worldTransform;

	Bound m_bound;

	Controller* m_controllers;

	Vector3 m_scale; // temporary
};

#include "Node.inl"

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
