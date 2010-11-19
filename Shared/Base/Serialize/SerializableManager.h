#ifndef zenic_SerializableManager_h
#define zenic_SerializableManager_h

#include "../Types.h"
#include "../Memory/DataPtr.h"

namespace zenic
{

class Serializable;

class SerializableManager
{
public:

	SerializableManager();
	~SerializableManager();

	void setPools(DataPtr< DataPtr<u8> > pools);
	DataPtr< DataPtr<u8 > > pools() const;

	void insert(Serializable* object, bool owned = false);
	void remove(Serializable* object);
	void release();

private:

	Serializable* m_first;
	Serializable* m_firstOwned;

	DataPtr< DataPtr<u8> > m_pools;
};

#include "SerializableManager.inl"

}

#endif // zenic_SerializableManager_h
