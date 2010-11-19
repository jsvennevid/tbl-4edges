#ifndef zenic_InBinarySerializer_h
#define zenic_InBinarySerializer_h

#include "InSerializer.h"

#include "SerializableAllocation.h"
#include "SerializableVersion.h"

#include "../Storage/Array.h"
#include "../Io/Stream.h"

namespace zenic
{
	class SerializableStructure;
}

namespace zenic
{

class InBinarySerializer : public InSerializer
{
public:

	InBinarySerializer();
	virtual ~InBinarySerializer();

	virtual void reset();
	virtual bool load(Stream& stream);

	void setCustomManager(SerializableManager* manager);

protected:

	enum
	{
		Version1 = 1,
		MaxVersion = Version1
	};

	enum Endian
	{
		LittleEndian,
		BigEndian
	};

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

	class Pool
	{
	public:

		void setData(DataPtr<u8> data);
		DataPtr<u8> data() const;

		void setAllocator(Allocator* allocator);
		Allocator* allocator() const;

		void setHandle(u32 handle);
		u32 handle() const;

		void setSize(u32 size);
		u32 size() const;

	private:

		DataPtr<u8> m_data;
		Allocator* m_allocator;
		u32 m_handle;
		u32 m_size;
	};

	class Object
	{
	public:

		void setFactory(SerializableFactory* factory);
		SerializableFactory* factory() const;

		void setOffset(u32 offset);
		u32 offset() const;

		void setObject(Serializable* object);
		Serializable* object() const;

	private:

		SerializableFactory* m_factory;
		u32 m_offset;
		Serializable* m_object;
	};

	class Block
	{
	public:

		Block() : m_data(0), m_offset(0), m_pool(0), m_structure(0), m_owner(0), m_allocator(0), m_align(0), m_size(0) {}

		bool load(Stream& stream);

		void setData(u8* data);
		u8* data() const;

		void setOffset(u32 offset);
		u32 offset() const;

		void setPool(u32 pool);
		u32 pool() const;

		u32 owner() const;
		SerializableStructure* structure() const;
		u32 allocator() const;
		u32 align() const;
		u32 size() const;

	private:

		u8* m_data;
		u32 m_offset;
		u32 m_pool;

		SerializableStructure* m_structure;
		u32 m_owner;
		u32 m_allocator;
		u32 m_align;
		u32 m_size;

	};

	DataPtr<Pool> m_pools;
	DataPtr<Object> m_objects;
	DataPtr<Block> m_blocks;

	SerializableManager* m_customManager;

	u32 m_endian;
	u32 m_blockIndex;
	u32 m_objectIndex;

	Stream* m_stream;
};

#include "InBinarySerializer.inl"

}

#endif
