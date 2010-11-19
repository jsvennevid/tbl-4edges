#ifndef zenic_XmlComment_h
#define zenic_XmlComment_h

#include "XmlNode.h"

namespace zenic
{

class XmlComment : public XmlNode
{
public:

	virtual Type type() const;
	virtual XmlNode* clone() const;
	virtual bool write(Stream& stream, uint depth) const;

private:
};

}

#endif
