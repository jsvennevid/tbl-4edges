#include "XmlText.h"

#include "../../Io/Stream.h"

namespace zenic
{

XmlNode::Type XmlText::type() const
{
	return XmlNode::Text;
}

bool XmlText::write(Stream& stream, uint /*depth*/) const
{
	stream << value();
	return true;
}

XmlNode* XmlText::clone() const
{
	return new XmlText;
}

}
