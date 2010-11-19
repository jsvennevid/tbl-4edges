#ifndef zenic_OutBinarySerializer_h
#define zenic_OutBinarySerializer_h

#include "OutSerializer.h"

#include "SerializableAllocation.h"
#include "SerializableVersion.h"

#include "../Storage/Array.h"
#include "../Io/BufferStream.h"

namespace zenic
{
	class SerializableStructure;
}

namespace zenic
{

class OutBinarySerializer : public OutSerializer
{
public:

	enum Endian
	{
		LittleEndian,
		BigEndian
	};

	OutBinarySerializer();
	virtual ~OutBinarySerializer();

	virtual void reset();
	virtual bool save(Stream& s);

	void setEndian(Endian endian);
	Endian endian() const;

protected:

	enum { Version = 1 };

	enum { SectorSize = 2048 };

	enum
	{
		MagicCookie = ('Z'<<24)|('N'<<16)|('I'<<8)|'C',
		MagicMarker = ('M'<<24)|('A'<<16)|('R'<<8)|'K'
	};

	virtual void pushGroup(const char* name, const char* type);
	virtual void popGroup();

	virtual void pushStructure(const char* name, SerializableStructure* structure, Pointer* ptr);
	virtual void popStructure();

	virtual void process(SerializableVersion& version);
	virtual void process(SerializableAllocation& allocation);
	virtual void process(const char* name, Serializable*& object);
	virtual void process(const char* name, Pointer* ptr, u32 elementSize);

	virtual void process(const char* name, f32& value);
	virtual void process(const char* name, bool& value);

	virtual void process(const char* name, s8& value);
	virtual void process(const char* name, u8& value);
	virtual void process(const char* name, c8& value);

	virtual void process(const char* name, s16& value);
	virtual void process(const char* name, u16& value);

	virtual void process(const char* name, s32& value);
	virtual void process(const char* name, u32& value);

	virtual void process(const char* name, s64& value);
	virtual void process(const char* name, u64& value);

private:

	class Block
	{
	public:

		Block();
		Block(const Block& block);

		void setOwner(u32 owner);
		u32 owner() const;

		void setStructure(SerializableStructure* structure);
		SerializableStructure* structure() const;

		void setAllocator(Allocator* allocator);
		Allocator* allocator() const;

		void setAlignment(u32 alignment);
		u32 alignment() const;

		void setPointer(Pointer* pointer);
		Pointer* pointer() const;

		void setElementSize(u32 elementSize);
		u32 elementSize() const;

	private:

		u32 m_owner;
		SerializableStructure* m_structure;
		Allocator* m_allocator;
		u32 m_alignment;

		Pointer* m_pointer;
		u32 m_elementSize;
	};

	class Object
	{
	public:
		Object(Serializable* instance = 0) : m_instance(instance), m_factory(0) {}

		Serializable* m_instance;
		SerializableFactory* m_factory;

		BufferStream m_stream;
	};

	Array<Allocator*> m_pools;
	Array<Object*> m_objects;
	Array<Block> m_blocks;

	Object* m_activeObject;
	uint m_activeIndex;

	Endian m_endian;
	SerializableAllocation m_activeAllocation;
};

#include "OutBinarySerializer.inl"

}

#endif
