#ifndef zenic_XmlBase_h
#define zenic_XmlBase_h

#include "../../Types.h"

/*

	zenic::XmlDocument Object Model

	Document 
	|- Element (container or leaf)
	|	|- Element (container or leaf)
	|	|- Text (leaf)
	|	|- Attributes (not in tree)
	|	|- Comment (leaf)
	|	|- Unknown (leaf)
	|- Comment (leaf)
	|- Unknown (leaf)
	|- Declaration (leaf)
		|- Attributes (not in tree)
*/

namespace zenic
{
	class Stream;
}

namespace zenic
{

class XmlBase
{
public:

	virtual ~XmlBase();

	virtual bool write(Stream& stream, uint depth) const = 0;

private:
};

}

#endif
