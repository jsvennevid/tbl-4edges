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
