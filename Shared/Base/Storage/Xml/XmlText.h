#ifndef zenic_XmlText_h
#define zenic_XmlText_h

#include "XmlNode.h"

namespace zenic
{

class XmlText : public XmlNode
{
public:

	virtual Type type() const;
	virtual bool write(Stream& stream, uint depth) const;
	virtual XmlNode* clone() const;

private:
};

}

#endif
