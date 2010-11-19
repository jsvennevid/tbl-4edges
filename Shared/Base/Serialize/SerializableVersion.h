#ifndef zenic_SerializableVersion_h
#define zenic_SerializableVersion_h

#include "../Types.h"

namespace zenic
{

class SerializableFactory;

class SerializableVersion
{
public:

	SerializableVersion(u32 version = 0, SerializableFactory* factory = 0);

	void setVersion(u32 version);
	u32 version() const;

	SerializableFactory* factory() const;

private:

	u32 m_version;
	SerializableFactory* m_factory;
};

#include "SerializableVersion.inl"

}

#endif
