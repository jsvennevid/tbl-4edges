#ifndef zenic_XmlDocument_h
#define zenic_XmlDocument_h

#include "XmlNode.h"

namespace zenic
{
	class XmlElement;
}

namespace zenic
{

class XmlDocument : public XmlNode
{
public:

	bool load(Stream& stream);
	bool save(Stream& stream);

	XmlElement* root() const;

	virtual Type type() const;
	virtual bool write(Stream& stream, uint depth) const;
	virtual XmlNode* clone() const;

private:
};

}

#endif
