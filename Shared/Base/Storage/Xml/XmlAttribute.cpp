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
