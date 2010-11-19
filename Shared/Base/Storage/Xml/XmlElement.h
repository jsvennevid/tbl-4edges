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
