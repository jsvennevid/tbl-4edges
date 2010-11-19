inline OutBinarySerializer::Block::Block()
{
	m_owner = 0;
	m_structure = 0;
	m_allocator = &HeapAllocator::allocator();
	m_alignment = 0;

	m_pointer = 0;
	m_elementSize = 0;
}

inline OutBinarySerializer::Block::Block(const Block& block)
{
	m_owner = block.m_owner;
	m_structure = block.m_structure;
	m_allocator = block.m_allocator;
	m_alignment = block.m_alignment;

	m_pointer = block.m_pointer;
	m_elementSize = block.m_elementSize;
}

inline void OutBinarySerializer::Block::setOwner(u32 owner)
{
	m_owner = owner;
}

inline u32 OutBinarySerializer::Block::owner() const
{
	return m_owner;
}

inline void OutBinarySerializer::Block::setStructure(SerializableStructure* structure)
{
	m_structure = structure;
}

inline SerializableStructure* OutBinarySerializer::Block::structure() const
{
	return m_structure;
}

inline void OutBinarySerializer::Block::setAllocator(Allocator* allocator)
{
	m_allocator = allocator;
}

inline Allocator* OutBinarySerializer::Block::allocator() const
{
	return m_allocator;
}

inline void OutBinarySerializer::Block::setAlignment(u32 alignment)
{
	m_alignment = alignment;
}

inline u32 OutBinarySerializer::Block::alignment() const
{
	return m_alignment;
}

inline void OutBinarySerializer::Block::setPointer(Pointer* pointer)
{
	m_pointer = pointer;
}

inline Serializer::Pointer* OutBinarySerializer::Block::pointer() const
{
	return m_pointer;
}

inline void OutBinarySerializer::Block::setElementSize(u32 elementSize)
{
	m_elementSize = elementSize;
}

inline u32 OutBinarySerializer::Block::elementSize() const
{
	return m_elementSize;
}
