#ifndef zenic_OutSerializer_h
#define zenic_OutSerializer_h

#include "Serializer.h"

namespace zenic
{

class OutSerializer : public Serializer
{
public:
	virtual bool save(Stream& s) = 0;
};

}

#endif
