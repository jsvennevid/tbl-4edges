#include "OutBinarySerializer.h"

#include "SerializableFactory.h"
#include "SerializableStructure.h"

namespace zenic
{

OutBinarySerializer::OutBinarySerializer()
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OutBinarySerializer::~OutBinarySerializer()
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::reset()
{
	Serializer::reset();

	m_pools.clear();
	// TODO: should this use inlined objects instead?
	for (uint i = 0, n = m_objects.count(); i < n; ++i)
		zenic_delete m_objects[i];
	m_objects.clear();
	m_blocks.clear();

	m_activeObject = 0;
	m_activeIndex = 0;

	// TODO: have a different value when compiling on big-endian
	m_endian = LittleEndian;
	m_activeAllocation = SerializableAllocation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool OutBinarySerializer::save(Stream& stream)
{
	// push default allocator

	m_pools.pushBack(&HeapAllocator::allocator());

	// add initial objects

	for (uint i = 0, n = count(); i < n; ++i)
	{
		Serializable* initial = (*this)[i];
		uint j,m;
		for (j = 0, m = m_objects.count(); j < m; ++j)
		{
			if (m_objects[j]->m_instance == initial)
				break;
		}

		if (j == m_objects.count())
			m_objects.pushBack(new Object(initial));
	}

	// serialize all objects

	for (uint i = 0; i < m_objects.count(); ++i)
	{
		m_activeIndex = i;
		m_activeObject = m_objects[i];
		ZENIC_ASSERT(m_activeObject);

		m_activeObject->m_instance->serialize(*this);
	}

	// register all memory allocators

	for (uint i = 0; i < m_blocks.count(); ++i)
	{
		uint j;
		for (j = 0; j < m_pools.count(); ++j)
		{
			if (m_pools[j] == m_blocks[i].allocator())
				break;
		}

		if (j == m_pools.count())
			m_pools.pushBack(m_blocks[i].allocator());
	}

	// *** START OF STREAM OUTPUT ***

	// write header

	stream.write32BE(MagicCookie);
	stream.write32BE(Version);
	stream.write32BE(m_endian);

	// write pool descriptors

	stream.write32BE(m_pools.count());
	for (uint i = 0, n = m_pools.count(); i < n; ++i)
		stream.write32BE(m_pools[i]->handle());

	// write object descriptors

	stream.write32BE(m_objects.count());
	for (uint i = 0; i < m_objects.count(); ++i)
	{
		// write factory identification for object

		SerializableFactory* factory = m_objects[i]->m_factory;
		ZENIC_ASSERT(factory);

		stream.write32BE(factory->host());
		stream.write32BE(factory->type());
		stream.write32BE(factory->size());
	}

	// write block descriptors

	stream.write32BE(m_blocks.count());
	for (uint i = 0; i < m_blocks.count(); ++i)
	{
		stream.write32BE(m_blocks[i].owner());

		if (m_blocks[i].structure())
		{
			stream.write32BE(m_blocks[i].structure()->identifier());

			SerializableFactory* factory = m_blocks[i].structure()->factory();

			stream.write32BE(factory->host());
			stream.write32BE(factory->type());
		}
		else
			stream.write32BE(0);

		stream.write32BE(m_blocks[i].allocator()->handle());
		stream.write32BE(m_blocks[i].alignment());
		if (m_blocks[i].structure())
			stream.write32BE(m_blocks[i].pointer()->m_count); 
		else
			stream.write32BE(m_blocks[i].pointer()->m_count * m_blocks[i].elementSize()); 
	}

	// write blocks

	for (uint i = 0; i < m_blocks.count(); ++i)
	{
		if (!m_blocks[i].pointer()->m_count || m_blocks[i].structure())
			continue;
		stream.write(m_blocks[i].pointer()->m_objects, m_blocks[i].elementSize() * m_blocks[i].pointer()->m_count);
	}

	// write objects

	for (uint i = 0; i < m_objects.count(); ++i)
	{
		// write object data

		DataPtr<u8> data = m_objects[i]->m_stream.buffer();
		stream.write32BE(data.count());

		stream.write(data);

		stream.write32BE(MagicMarker);
	}

	// write top objects

	stream.write32BE(count());
	for (uint i = 0, n = count(); i < n; ++i)
	{
		Serializable* object = (*this)[i];

		uint j,m;
		for (j = 0, m = m_objects.count(); j < m; ++j)
		{
			if (m_objects[j]->m_instance == object)
				break;
		}

		ZENIC_ASSERT(j < m_objects.count());

		stream.write32BE(j);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::pushGroup(const char* /*name*/, const char* /*type*/)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::popGroup()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::pushStructure(const char* /*name*/, SerializableStructure* structure, Pointer* ptr)
{
	Block block;

	block.setOwner(m_activeIndex);
	block.setStructure(structure);
	block.setAllocator(&m_activeAllocation.allocator());
	block.setAlignment(m_activeAllocation.alignment());
	block.setPointer(ptr);
	block.setElementSize(0);

	m_blocks.pushBack(block);

	m_activeAllocation = SerializableAllocation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::popStructure()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, f32& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(SerializableVersion& version)
{
	ZENIC_ASSERT(m_activeObject);

	if (!m_activeObject->m_factory)
		m_activeObject->m_factory = version.factory();

	u32 versionNumber = version.version();
	m_activeObject->m_stream.write(&versionNumber, sizeof(versionNumber));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(SerializableAllocation& allocation)
{
	m_activeAllocation = allocation;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, Serializable*& object)
{
	ZENIC_ASSERT(m_activeObject);

	u32 index = u32(-1);

	if (object)
	{
		uint i;

		for (i = 0; i < m_objects.count(); ++i)
		{
			if (m_objects[i]->m_instance == object)
				break;
		}

		if (i == m_objects.count())
			m_objects.pushBack(new Object(object));

		index = u32(i);
	}

	m_activeObject->m_stream.write(&index, sizeof(index));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, Pointer* ptr, u32 elementSize)
{
	Block block;

	block.setOwner(m_activeIndex);
	block.setStructure(0);
	block.setAllocator(&m_activeAllocation.allocator());
	block.setAlignment(m_activeAllocation.alignment());
	block.setPointer(ptr);
	block.setElementSize(elementSize);

	m_blocks.pushBack(block);

	m_activeAllocation = SerializableAllocation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, bool& value)
{
	u32 trueval = u32(value);
	m_activeObject->m_stream.write(&trueval, sizeof(trueval));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, s8& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, u8& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, c8& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, s16& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, u16& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, s32& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, u32& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, s64& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutBinarySerializer::process(const char* /*name*/, u64& value)
{
	m_activeObject->m_stream.write(&value, sizeof(value));
}

}
