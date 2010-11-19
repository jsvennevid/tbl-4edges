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
