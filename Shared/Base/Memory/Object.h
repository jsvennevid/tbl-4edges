#ifndef zenic_Object_h
#define zenic_Object_h

#include "../Types.h"
#include "../Serialize/Serializable.h"
#include "../Serialize/Rtti.h"

namespace zenic
{

class Object : public Serializable
{
	ZENIC_ROOT_RTTI

public:

	virtual ~Object();

protected:

	Object();

private:
};

#include "Object.inl"

}

#endif
