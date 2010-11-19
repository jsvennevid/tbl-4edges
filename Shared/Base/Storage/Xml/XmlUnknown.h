#ifndef zenic_XmlUnknown_h
#define zenic_XmlUnknown_h

#include "XmlNode.h"

namespace zenic
{

class XmlUnknown : public XmlNode
{
public:

	XmlUnknown();
	~XmlUnknown();

	virtual Type type() const;
	virtual bool write(Stream& stream, uint depth) const;
	virtual XmlNode* clone() const;

private:
};

}

#endif
