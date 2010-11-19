#include "XmlDocument.h"

#include "../../Io/Stream.h"

namespace zenic
{

bool XmlDocument::load(Stream& /*stream*/)
{
	return false;
}

bool XmlDocument::save(Stream& stream)
{
	return write(stream,0);
}

XmlNode::Type XmlDocument::type() const
{
	return Document;
}

bool XmlDocument::write(Stream& stream, uint depth) const
{
	for (XmlNode* child = firstChild(); child; child = child->next())
	{
		child->write(stream,depth);
		stream << endl;
	}
	return true;
}

XmlNode* XmlDocument::clone() const
{
	return new XmlDocument;
}

XmlElement* XmlDocument::root() const
{
	for (XmlNode* child = firstChild(); child; child = child->next())
	{
		if (child->toElement())
			return reinterpret_cast<XmlElement*>(child);
	}
	return 0;
}

}
