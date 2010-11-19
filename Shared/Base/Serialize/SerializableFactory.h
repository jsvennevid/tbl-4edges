#ifndef zenic_SerializableFactory_h
#define zenic_SerializableFactory_h

#include "../Types.h"

namespace zenic
{
	class Serializable;
	class SerializableStructure;
}

namespace zenic
{

class SerializableFactory
{
public:

	typedef u32 Identifier;

	SerializableFactory(Identifier host, Identifier type, u32 size);
	virtual ~SerializableFactory();

	virtual Serializable* create(void* ptr) const = 0;

	Identifier host() const;
	Identifier type() const;
	u32 size() const;

	SerializableFactory* next() const;
	static SerializableFactory* first();

	static SerializableFactory* find(Identifier host, Identifier type);
	SerializableStructure* findStructure(Identifier identifier) const;

	void setStructures(SerializableStructure* structures);
	SerializableStructure* structures() const;

private:

	Identifier m_host;
	Identifier m_type;
	u32 m_size;

	SerializableFactory* m_next;
	static SerializableFactory* ms_first;

	SerializableStructure* m_structures;
};

#include "SerializableFactory.inl"

}

#define SERIALIZABLE_ID(b1,b2,b3,b4) (u32((b1<<24)|(b2<<16)|(b3<<8)|(b4)))

#define ZENIC_SERIALIZABLE_FACTORY \
private: \
class Factory : public zenic::SerializableFactory \
{ \
public: \
	Factory(Identifier host, Identifier type, u32 size) : zenic::SerializableFactory(host,type,size) {} \
	virtual Serializable* create(void* ptr) const; \
}; \
static Factory ms_factory; \
public: \
static Factory* factory() { return &ms_factory; } \
virtual zenic::SerializableFactory* serializableFactory()

#define ZENIC_CREATE_SERIALIZABLE_FACTORY(name,hostid,typeid) \
name::Factory name::ms_factory((hostid),(typeid),sizeof(name));\
zenic::Serializable* name::Factory::create(void* ptr) const { return new (ptr) name; } \
zenic::SerializableFactory* name::serializableFactory() { return factory(); }

#define ZENIC_REFERENCE_SERIALIZABLE_FACTORY(name) \
static zenic::SerializableFactory* __factory_reference_##name = name::factory()

#endif
