#ifndef zenic_XmlDeclaration_h
#define zenic_XmlDeclaration_h

#include "XmlElement.h"

namespace zenic
{

class XmlDeclaration : public XmlElement
{
public:

	virtual Type type() const;
	virtual bool write(Stream& stream, uint depth) const;
	virtual XmlNode* clone() const;
};

}

#endif
