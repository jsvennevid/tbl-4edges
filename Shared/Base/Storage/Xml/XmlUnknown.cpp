#include "XmlUnknown.h"

#include "../../Io/Stream.h"

namespace zenic
{

XmlUnknown::XmlUnknown()
{
}

XmlUnknown::~XmlUnknown()
{
}

XmlNode::Type XmlUnknown::type() const
{
	return Unknown;
}

bool XmlUnknown::write(Stream& stream, uint depth) const
{
	for (uint i = 0; i < depth; ++i)
		stream << "    ";
	stream << value();

	return true;
}

XmlNode* XmlUnknown::clone() const
{
	return new XmlUnknown;
}

}
