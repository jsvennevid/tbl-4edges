#ifndef zenic_SerializableStructure_h
#define zenic_SerializableStructure_h

#include "SerializableFactory.h"

#include "../Types.h"

namespace zenic
{

class SerializableStructure
{
public:

	typedef u32 Identifier;

	SerializableStructure(SerializableFactory* factory, Identifier identifier, u32 size);
	virtual ~SerializableStructure();

	virtual void* create(void* ptr, uint count) const = 0;

	Identifier identifier() const;
	u32 size() const;

	SerializableFactory* factory() const;

	SerializableStructure* next() const;

private:

	Identifier m_identifier;
	u32 m_size;

	SerializableFactory* m_factory;
	SerializableStructure* m_next;
};

#include "SerializableStructure.inl"

}

#define ZENIC_SERIALIZABLE_STRUCTURE \
private: \
class Structure : public zenic::SerializableStructure \
{ \
public: \
	Structure(zenic::SerializableFactory* factory, Identifier identifier, u32 size ) : zenic::SerializableStructure( factory, identifier, size) {} \
	virtual void* create(void* ptr, uint count) const; \
}; \
static Structure ms_structure; \
public: \
static Structure* structure() { return &ms_structure; }\
void serialize(zenic::Serializer& s)

#define ZENIC_CREATE_SERIALIZABLE_STRUCTURE(owner,name,identifier) \
owner::name::Structure owner::name::ms_structure(&(owner::ms_factory),(identifier),sizeof(owner::name)); \
void* owner::name::Structure::create(void* ptr, uint count ) const { u8* curr = static_cast<u8*>(ptr); for ( uint i = 0; i < count; ++i) { new (curr) name; curr += sizeof(name); } return ptr; }

#endif
