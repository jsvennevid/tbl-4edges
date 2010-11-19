#include "XmlNode.h"

namespace zenic
{

XmlNode::XmlNode() : m_parent(0), m_firstChild(0), m_lastChild(0), m_prev(0), m_next(0), m_userData(0)
{
}

XmlNode::~XmlNode()
{
	clear();
}

void XmlNode::clear()
{
	XmlNode* node = m_firstChild;
	XmlNode* temp = 0;

	while (node)
	{
		temp = node;
		node = node->m_next;
		delete temp;
	}

	m_firstChild = m_lastChild = 0;
}

void XmlNode::addChild(XmlNode* child)
{
	insertChild(0, child);
}

void XmlNode::insertChild(XmlNode* after, XmlNode* child)
{
	ZENIC_ASSERT(child);
	ZENIC_ASSERT(!child->m_parent);

	if (!after)
	{
		if (m_lastChild)
		{
			child->m_prev = m_lastChild;
			child->m_next = 0;

			m_lastChild->m_next = child;
			m_lastChild = child;
		}
		else
		{
			child->m_prev = 0;
			child->m_next = 0;

			m_firstChild = m_lastChild = child;
		}
	}
	else
	{
		ZENIC_ASSERT(after->m_parent == this);

		if (after->m_prev)
		{
			child->m_prev = after->m_prev;
			child->m_next = after;

			after->m_prev->m_next = child;
			after->m_prev = child;			
		}
		else
		{
			child->m_prev = 0;
			child->m_next = after;

			m_firstChild = child;
			after->m_prev = child;
		}
	}

	child->m_parent = this;
}

void XmlNode::replaceChild(XmlNode* node, XmlNode* child)
{
	XmlNode* after = node->m_next;

	removeChild(node);
	insertChild(after, child);
}

void XmlNode::removeChild(XmlNode* child)
{
	ZENIC_ASSERT(child);
	ZENIC_ASSERT(child->m_parent == this);

	if (child->m_prev)
	{
		if (child->m_next)	
		{
			child->m_prev->m_next = child->m_next;
			child->m_next->m_prev = child->m_prev;
		}
		else
		{
			child->m_prev->m_next = 0;
			m_lastChild = child->m_prev;
		}
	}
	else
	{
		if (child->m_next)
		{
			child->m_next->m_prev = 0;
			m_firstChild = child->m_next;
		}
		else
		{
			m_firstChild = m_lastChild = 0;
		}
	}

	child->m_prev = 0;
	child->m_next = 0;

	child->m_parent = 0;
}

}
