#include "InBinarySerializer.h"

#include "SerializableFactory.h"
#include "SerializableStructure.h"

#include "../Io/Log.h"

#include <string.h>

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

InBinarySerializer::InBinarySerializer()
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

InBinarySerializer::~InBinarySerializer()
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::reset()
{
	Serializer::reset();

	m_pools.free();
	m_objects.free();
	m_blocks.free();

	m_customManager = 0;

	m_endian = LittleEndian;
	m_blockIndex = 0;
	m_objectIndex = 0;

	m_stream = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool InBinarySerializer::load(Stream& stream)
{
	reset();

	m_stream = &stream;

	// read header

	u32 cookie;
	if (!stream.read32BE(cookie) || (MagicCookie != cookie))
		return false;

	u32 version;
	if (!stream.read32BE(version) || (MaxVersion < version))
		return false;

	if (!stream.read32BE(m_endian))
		return false;

	// read pool descriptors

	u32 pools;
	if (!stream.read32BE(pools) || !pools)
	{
		ZENIC_ERROR("Failed while reading memory-pools");
		return false;
	}

	m_pools.allocate(pools);
	for (uint i = 0; i < pools; ++i)
	{
		u32 handle;
		if (!stream.read32BE(handle))
			return false;

		Allocator* allocator = Allocator::find(handle);

		if (!allocator)
		{
			ZENIC_ERROR("Could not locate allocator for ID 0x" << hex << setw(8) << handle);
			return false;
		}

		m_pools[i].setHandle(handle);
		m_pools[i].setSize(0);
		m_pools[i].setAllocator(allocator);
	}

	// read object descriptors

	u32 objects;
	if (!stream.read32BE(objects))
		return false;

	u32 currOffset = 0;
	m_objects.allocate(objects);
	for (uint i = 0; i < objects; ++i)
	{
		u32 host, type, size;

		if (!stream.read32BE(host ) || !stream.read32BE(type) || !stream.read32BE( size))
			return false;

		// lookup factory

		SerializableFactory* factory = SerializableFactory::find(host, type);

		if (!factory)
		{
			char hostString[5];
			char typeString[5];

			hostString[0] = char((host >> 24) & 0xff);
			hostString[1] = char((host >> 16) & 0xff);
			hostString[2] = char((host >> 8) & 0xff);
			hostString[3] = char((host >> 0) & 0xff);
			hostString[4] = 0;	

			typeString[0] = char((type >> 24) & 0xff);
			typeString[1] = char((type >> 16) & 0xff);
			typeString[2] = char((type >> 8) & 0xff);
			typeString[3] = char((type >> 0) & 0xff);
			typeString[4] = 0;	

			ZENIC_ERROR("Could not find factory for " << hostString << ":" << typeString);
			return false;
		}

		ZENIC_VERIFY(factory->size() == size, "Size of object " << i << " differs from factory");

		m_objects[i].setFactory(factory);
		m_objects[i].setOffset(currOffset);

		currOffset += (factory->size() + (OBJECT_ALIGN-1)) & ~(OBJECT_ALIGN-1);
	}

	m_pools[0].setSize(currOffset);

	// read block descriptors

	u32 blocks;
	if (!stream.read32BE(blocks))
		return false;

	m_blocks.allocate(blocks);
	for (uint i = 0; i < blocks; ++i)
	{
		if (!m_blocks[i].load(stream))
			return false;

		u32 memorySize = u32(-1);

		if (m_blocks[i].structure())
			memorySize = m_blocks[i].size() * m_blocks[i].structure()->size();
		else
			memorySize = m_blocks[i].size();

		// locate pool

		m_blocks[i].setPool(u32(-1));
		uint j;
		for (j = 0; j < pools; ++j)
		{
			if (m_pools[j].handle() == m_blocks[i].allocator())
			{
				m_blocks[i].setPool(j);
				break;
			}				
		}

		if (m_blocks[i].pool() == u32(-1))
		{
			ZENIC_ERROR("Could not locate memory pool for structure");
			return false;
		}

		// get aligned offset for memory-block
		u32 offset = ((m_pools[j].size() + m_blocks[i].align()-1)/m_blocks[i].align())*m_blocks[i].align();
		m_blocks[i].setOffset(offset);

		m_pools[j].setSize(offset + memorySize);
	}

	// allocate memory for loading

	SerializableManager* manager = 0;

	if (m_customManager)
	{
		// verify that memorymanager contains enough memory to load all data

		ZENIC_ASSERT_DESC(0, "Complete support for custom memory manager");

		for (uint i = 0; i < pools; ++i)
		{
			uint j;
			DataPtr< DataPtr<u8> > memory = m_customManager->pools();

			for (j = 0; j < memory.count(); ++j)
			{
				if (memory[j].allocator() == m_pools[i].allocator())
				{
					if (memory[j].count() < m_pools[i].size())
					{
						ZENIC_ERROR("Memory-block is too small");
						return false;
					}
					break;
				}
			}

			if (j == memory.count())
			{
				ZENIC_ERROR("Memory-type not found");
				return false;
			}
		}

		manager = m_customManager;
	}
	else
	{
		// create new memory manager that can contain all data

		manager = zenic_new SerializableManager;
		ZENIC_ASSERT(manager);

		DataPtr< DataPtr<u8> > memory;

		memory.allocate(pools);
		for (uint i = 0; i < pools; ++i)
		{
			memory[i].allocate(m_pools[i].size(), m_pools[i].allocator());
			m_pools[i].setData(memory[i]);
		}

		manager->setPools(memory);
	}

	// create objects

	for (uint i = 0; i < objects; ++i)
	{
		u8* ptr = m_pools[0].data() + m_objects[i].offset();
		Serializable* object = m_objects[i].factory()->create(ptr);
		manager->insert(object,true);
		m_objects[i].setObject(object);
	}

	// setup & load blocks

	for (uint i = 0; i < blocks; ++i)
	{
		if (!m_blocks[i].size())
			continue;

		u8* memory = m_pools[m_blocks[i].pool()].data() + m_blocks[i].offset();

		if (!m_blocks[i].structure())
		{
			// standard block, load data

			if (!stream.read(memory, m_blocks[i].size()))
			{
				ZENIC_ERROR("Premature end of stream while reading block");
				if (!m_customManager)
					delete manager;
				return false;
			}
		}
		else
		{
			// dynamic structure, data is inlined
			m_blocks[i].structure()->create(memory, m_blocks[i].size());
		}

		m_blocks[i].setData(memory);
	}

	// read objects

	m_blockIndex = 0;
	for (uint i = 0; i < objects; ++i)
	{
		// read object size

		u32 size;
		if (!stream.read32BE(size))
			return false;

		// load data into object

		m_objects[i].object()->serialize(*this);

		// read marker

		u32 marker;
		if (!stream.read32BE(marker) || (MagicMarker != marker))
			return false;
	}

	// read top objects

	u32 topCount;
	if (!stream.read32BE(topCount))
		return false;

	for (uint i = 0; i < topCount; ++i)
	{
		u32 index;
		if (!stream.read32BE(index))
			return false;

		add(m_objects[index].object());
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool InBinarySerializer::Block::load(Stream& stream)
{
	u32 host, type, id;

	bool success;

	success = stream.read32BE(m_owner);
	success = stream.read32BE(id) && success;
	if (id)
	{
		stream.read32BE(host);
		stream.read32BE(type);

		SerializableFactory* factory = SerializableFactory::find(host, type);
		ZENIC_ASSERT(factory);

		if (!factory)
		{
			ZENIC_ERROR("Could not locate factory for serialized structure");
			return false;
		}

		m_structure = factory->findStructure(id);
		ZENIC_ASSERT(id);

		if (!m_structure)
		{
			ZENIC_ERROR( "Could not locate serialized structure ");
			return false;
		}
	}
	success = success && stream.read32BE(m_allocator);
	success = success && stream.read32BE(m_align);
	success = success && stream.read32BE(m_size);

	return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::pushGroup(const char* /*name*/, const char* /*type*/)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::popGroup()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::pushStructure(const char* /*name*/, SerializableStructure* /*structure*/, Pointer* ptr)
{
	uint i = m_blockIndex;

	ZENIC_ASSERT(m_blocks[i].structure());

	ptr->m_objects = m_blocks[i].data();
	ptr->m_count = m_blocks[i].size();
	ptr->m_allocator = 0;

	m_blockIndex++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::popStructure()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(SerializableVersion& version)
{
	u32 versionNumber;
	m_stream->read(&versionNumber, sizeof(versionNumber));
	version.setVersion(versionNumber);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(SerializableAllocation& /*allocation*/)
{
	// not processed by serializer
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, Serializable*& object)
{
	u32 index;

	m_stream->read(&index, sizeof(index));

	if (index != u32(-1))
	{
		ZENIC_ASSERT(index < m_objects.count());

		object = m_objects[index].object();
	}	
	else
	{
		object = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, Pointer* ptr, u32 elementSize)
{
	uint i = m_blockIndex;

	ZENIC_ASSERT(!m_blocks[i].structure());
	ZENIC_ASSERT((m_blocks[i].size() / elementSize) * elementSize == m_blocks[i].size());

	ptr->m_objects = m_blocks[i].data();
	ptr->m_count = m_blocks[i].size() / elementSize;
	ptr->m_allocator = 0;

	m_blockIndex++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, f32& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, bool& value)
{
	u32 trueval;
	m_stream->read(&trueval, sizeof(trueval));
	value = trueval ? true : false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, s8& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, u8& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, c8& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, s16& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, u16& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, s32& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, u32& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, s64& value)
{
	m_stream->read(&value, sizeof(value));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InBinarySerializer::process(const char* /*name*/, u64& value)
{
	m_stream->read(&value, sizeof(value));
}

}
