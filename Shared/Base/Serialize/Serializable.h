#ifndef zenic_Serializable_h
#define zenic_Serializable_h

#include "SerializableManager.h"

#include "../Memory/HeapAllocator.h"

namespace zenic
{
	class Serializer;
	class SerializableFactory;
}

namespace zenic
{

class Serializable
{
public:
	friend class SerializableManager;

	Serializable();
	virtual ~Serializable();

	void* operator new(size_t s);
	void* operator new(size_t s, void* p);

	void operator delete(void* ptr);
	void operator delete(void* ptr, void* p);

	virtual SerializableFactory* serializableFactory() = 0;
	virtual void serialize(Serializer& s) = 0;

	SerializableManager* manager() const;

private:

	SerializableManager* m_manager;

	Serializable* m_nextManaged;
};

#include "Serializable.inl"

}

#endif
