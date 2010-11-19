#include "XmlDeclaration.h"
#include "XmlAttribute.h"

#include "../String.h"
#include "../../Io/Stream.h"

namespace zenic
{

XmlNode::Type XmlDeclaration::type() const
{
	return Declaration;
}

bool XmlDeclaration::write(Stream& stream, uint depth) const
{
	stream << "<?" << value() << " ";

	for (XmlAttribute* attribute = firstAttribute(); attribute; attribute = attribute->next())
	{
		attribute->write(stream,depth);
		stream << " ";
	}

	stream << "?>";

	return true;
}

XmlNode* XmlDeclaration::clone() const
{
	return new XmlDeclaration;
}

}
