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
