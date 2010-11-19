#include "XmlAttribute.h"

#include "../../Io/Stream.h"
#include "../../Io/BufferStream.h"

namespace zenic
{

bool XmlAttribute::write(Stream& stream, uint /*depth*/) const
{
	if (value().find("\"") < 0)
		stream << name() << "=\"" << value() << "\"";
	else
		stream << name() << "='" << value() << "'";
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void XmlAttribute::setValue(int value)
{
	BufferStream temp;
	temp << value;

	setValue(temp.string());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void XmlAttribute::setValue(f32 value)
{
	BufferStream temp;
	temp << value;

	setValue(temp.string());
}

}
