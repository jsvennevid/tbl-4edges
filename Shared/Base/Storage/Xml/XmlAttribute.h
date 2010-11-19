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
