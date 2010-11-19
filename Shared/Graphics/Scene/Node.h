///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
