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
#ifndef zenic_XmlNode_h
#define zenic_XmlNode_h

#include "XmlBase.h"

#include "../String.h"
#include "../../Debug/Assert.h"

namespace zenic
{
	class XmlDocument;
	class XmlElement;
	class XmlComment;
	class XmlUnknown;
	class XmlText;
	class XmlDeclaration;
}

namespace zenic
{

class XmlNode : public XmlBase
{
public:

	enum Type
	{
		Document,
		Element,
		Comment,
		Unknown,
		Text,
		Declaration,

		NumTypes
	};

	XmlNode();
	virtual ~XmlNode();

	const String& value() const;
	void setValue(const String& value);

	bool isEmpty() const;
	void clear();

	XmlNode* parent() const;

	XmlNode* firstChild() const;
	XmlNode* lastChild() const;

	void addChild(XmlNode* child);
	void insertChild(XmlNode* after, XmlNode* child);
	void replaceChild(XmlNode* node, XmlNode* child);
	void removeChild(XmlNode* child);

	XmlNode* prev() const;
	XmlNode* next() const;

	XmlNode* iterate(XmlNode* child) const;

	XmlDocument* document() const;

	virtual Type type() const = 0;

	XmlDocument* toDocument();
	XmlElement* toElement();
	XmlComment* toComment();
	XmlUnknown* toUnknown();
	XmlText* toText();
	XmlDeclaration* toDeclaration();

	virtual XmlNode* clone() const = 0;

	void* userData() const;
	void setUserData(void* userData);

private:

	XmlDocument* m_document;
	XmlNode* m_parent;

	XmlNode* m_firstChild;
	XmlNode* m_lastChild;

	String m_value;

	XmlNode* m_prev;
	XmlNode* m_next;

	void* m_userData;
};

#include "XmlNode.inl"

}

#endif
