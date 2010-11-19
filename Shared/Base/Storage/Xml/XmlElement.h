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
#ifndef zenic_XmlElement_h
#define zenic_XmlElement_h

#include "XmlNode.h"

namespace zenic
{
	class XmlAttribute;
}

namespace zenic
{

class XmlElement : public XmlNode
{
public:

	XmlElement();
	virtual ~XmlElement();

	virtual Type type() const;
	virtual bool write(Stream& stream, uint depth) const;
	virtual XmlNode* clone() const;

	XmlAttribute* attribute(const String& name) const;

	void setAttribute(const String& name, const String& value);
	void setAttribute(const String& name, int value);
	void setAttribute(const String& name, f32 value);

	void removeAttribute(const String& name);
	void removeAttribute(XmlAttribute* attribute);

	XmlAttribute* firstAttribute() const;
	XmlAttribute* lastAttribute() const;

private:

	void attachAttribute(XmlAttribute* attribute);

	XmlAttribute* m_firstAttribute;
	XmlAttribute* m_lastAttribute;
};

#include "XmlElement.inl"

}

#endif
