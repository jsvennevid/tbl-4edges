#ifndef zenic_InSerializer_h
#define zenic_InSerializer_h

#include "Serializer.h"

namespace zenic
{

class InSerializer : public Serializer
{
public:
	virtual bool load(Stream& s) = 0;
};

}

#endif
