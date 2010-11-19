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

#include "Node.h"

#include "Controller.h"

#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Base/Memory/Prefetch.h>
#include <Shared/Base/Io/Log.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(Node, SERIALIZABLE_ID('Z','N','I','C'), SERIALIZABLE_ID('N','O','D','E'))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node::~Node()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Node::collect(Array<Node*>& nodes, Type type)
{
	if (isType(type))
		nodes.pushBack(this);

	for (Node* currChild = firstChild(); currChild; currChild = currChild->nextSibling())
		currChild->collect(nodes, type);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Node::isType(Type type)
{
	if (type == NodeType)
		return true;

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Node::update(float time)
{
	updateControllers(time);
	updateWorldTransform();

	// update all children & bounds

	Bound newBound;

	for (Node* currChild = firstChild(); currChild; currChild = currChild->nextSibling())
	{
		//ZENIC_PREFETCH_READ(currChild->nextSibling());
		currChild->update(time);
		newBound = newBound.merge(currChild->bound());
	}

	m_bound = newBound;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Node::updateControllers(float time)
{
	// TODO: Handle more controllers?

	if (m_controllers)
	{
		m_controllers->update(time, m_transform);
		Matrix4 scale;

		//const Vector3& pos = m_transform.position();
		//ZENIC_INFO(pos.x << " " << pos.y << " " << pos.z);

		scale = Matrix4::identity();
		scale.setScale(m_scale);
		m_transform = scale * m_transform;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Node::serialize(Serializer& s)
{
	SerializableVersion version(2, factory());

	s << version;

	s.descriptor(SERIALIZER_NAME("m_parent")) << m_parent;
	s.descriptor(SERIALIZER_NAME("m_firstChild")) << m_firstChild;
	s.descriptor(SERIALIZER_NAME("m_lastChild")) << m_lastChild;
	s.descriptor(SERIALIZER_NAME("m_prevSibling")) << m_prevSibling;
	s.descriptor(SERIALIZER_NAME("m_nextSibling")) << m_nextSibling;

	m_transform.serialize(s, SERIALIZER_NAME("m_transform"));
	m_worldTransform.serialize(s, SERIALIZER_NAME("m_worldTransform"));

	m_bound.serialize(s, SERIALIZER_NAME("m_bound"));

	s.descriptor(SERIALIZER_NAME("m_controllers")) << m_controllers;

	s.descriptor(SERIALIZER_NAME("m_scale.x")) << m_scale.x;
	s.descriptor(SERIALIZER_NAME("m_scale.y")) << m_scale.y;
	s.descriptor(SERIALIZER_NAME("m_scale.z")) << m_scale.z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Node::attachChild(Node* child)
{
	insertChild(m_lastChild,child);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Node::insertChild(Node* sibling, Node* child)
{
	if (sibling)
	{
		ZENIC_ASSERT(sibling->parent() == this);

		if (sibling->m_nextSibling)
		{
			child->m_nextSibling = sibling->m_nextSibling;
			child->m_prevSibling = sibling;

			child->m_nextSibling->m_prevSibling = child;
			child->m_prevSibling->m_nextSibling = child;
		}
		else
		{
			m_lastChild->m_nextSibling = child;
			child->m_prevSibling = m_lastChild;
			m_lastChild = child;
		}
	}
	else
	{
		if (m_lastChild)
		{
			child->m_nextSibling = m_firstChild;
			m_firstChild->m_prevSibling = child;
			m_firstChild = child;
		}
		else
		{
			m_firstChild = m_lastChild = child;
		}
	}

	child->setParent(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Node::detachChild(Node* child)
{
	ZENIC_ASSERT(child);
	ZENIC_ASSERT(child->parent() == this);

	if (child->m_prevSibling)
	{
		if (child->m_nextSibling)
		{
			child->m_prevSibling->m_nextSibling = child->m_nextSibling;
			child->m_nextSibling->m_prevSibling = child->m_prevSibling;
		}
		else
		{
			ZENIC_ASSERT(child == m_lastChild);

			m_lastChild = m_lastChild->m_prevSibling;
			m_lastChild->m_nextSibling = 0;
		}
	}
	else
	{
		ZENIC_ASSERT(child == m_firstChild);

		if (child->m_nextSibling)
		{
			m_firstChild = m_firstChild->m_nextSibling;
			m_firstChild->m_prevSibling = 0;
		}
		else
		{
			m_firstChild = m_lastChild = 0;
		}
	}

	child->m_prevSibling = 0;
	child->m_nextSibling = 0;

	child->setParent(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
