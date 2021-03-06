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
#include "XmlElement.h"

#include "XmlAttribute.h"
#include "../../Io/Stream.h"


namespace zenic
{

XmlElement::~XmlElement()
{
	XmlAttribute* curr = m_firstAttribute;
	XmlAttribute* next = 0;

	for (; curr ; curr = next)
	{
		next = curr->next();
		zenic_delete curr;
	}
}

XmlNode::Type XmlElement::type() const
{
	return Element;
}

bool XmlElement::write(Stream& stream, uint depth) const
{
	for (uint i = 0; i < depth; ++i)
		stream << "    ";

	stream << "<" << value();

	for (XmlAttribute* attribute = firstAttribute(); attribute; attribute = attribute->next())
	{
		stream << " ";
		attribute->write(stream,depth);
	}

	XmlNode* child = firstChild();
	if (!child)
	{
		stream << "/>";
	}
	else if ((child == lastChild()) && child->toText())
	{
		stream << ">";
		child->write(stream,depth+1);
		stream << "</" << value() << ">";
	}
	else
	{
		stream << ">";

		for (; child; child = child->next())
		{
			if (!child->toText())
				stream << endl;
			child->write(stream,depth+1);
		}
		stream << endl;
		for (uint i = 0; i < depth; ++i)
			stream << "    ";
		stream << "</" << value() << ">";
	}

	return true;
}

XmlNode* XmlElement::clone() const
{
	return new XmlElement;
}

XmlAttribute* XmlElement::attribute(const String& name) const
{
	for (XmlAttribute* curr = firstAttribute(); curr; curr = curr->next())
	{
		if (curr->name() == name)
			return curr;
	}

	return 0;
}

void XmlElement::setAttribute(const String& name, const String& value)
{
	XmlAttribute* attr = attribute(name);

	if (!attr)
	{
		attr = zenic_new XmlAttribute(name);
		attachAttribute(attr);
	}

	attr->setValue(value);
}

void XmlElement::setAttribute(const String& name, int value)
{
	XmlAttribute* attr = attribute(name);

	if (!attr)
	{
		attr = zenic_new XmlAttribute(name);
		attachAttribute(attr);
	}

	attr->setValue(value);
}

void XmlElement::setAttribute(const String& name, f32 value)
{
	XmlAttribute* attr = attribute(name);

	if (!attr)
	{
		attr = zenic_new XmlAttribute(name);
		attachAttribute(attr);
	}

	attr->setValue(value);
}

void XmlElement::removeAttribute(const String& name)
{
	XmlAttribute* attr = attribute(name);
	removeAttribute(attr);
}

void XmlElement::removeAttribute(XmlAttribute* attribute)
{
	if (attribute)
	{
		if (attribute->prev())
		{
			if (attribute->next())
			{
				attribute->next()->m_prev = attribute->prev();
				attribute->prev()->m_next = attribute->next();
			}
			else
			{
				attribute->prev()->m_next = 0;
				m_lastAttribute = attribute->prev();
			}
		}
		else
		{
			if (attribute->next())
			{
				attribute->next()->m_prev = 0;
				m_firstAttribute = attribute->next();
			}
			else
			{
				m_firstAttribute = 0;
				m_lastAttribute = 0;
			}
		}

		zenic_delete attribute;
	}
}

void XmlElement::attachAttribute(XmlAttribute* attribute)
{
	if (!m_firstAttribute)
	{
		m_firstAttribute = attribute;
		m_lastAttribute = attribute;
	}
	else
	{
		m_lastAttribute->m_next = attribute;
		attribute->m_prev = m_lastAttribute;
		m_lastAttribute = attribute;
	}
}

}
