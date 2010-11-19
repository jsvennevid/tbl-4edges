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
#ifndef zenic_XmlAttribute_h
#define zenic_XmlAttribute_h

#include "XmlBase.h"
#include "../String.h"

namespace zenic
{
	class XmlDocument;
}

namespace zenic
{

class XmlAttribute : public XmlBase
{
public:
	friend class XmlElement;

	XmlAttribute();
	XmlAttribute(const String& name);

	const String& name() const;
	void setName(const String& name);

	const String& value() const;
	void setValue(const String& value);
	void setValue(int value);
	void setValue(f32 value);

	bool toInteger(int& i);
	bool toFloat(f32& i);

	virtual bool write(Stream& stream, uint depth) const;

	XmlDocument* document() const;
	void setDocument(XmlDocument* document);

	XmlAttribute* prev() const;
	XmlAttribute* next() const;

private:

	XmlDocument* m_document;

	String m_name;
	String m_value;

	XmlAttribute* m_prev;
	XmlAttribute* m_next;
};

#include "XmlAttribute.inl"

}

#endif
