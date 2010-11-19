#ifndef zenic_SerializableAllocation_h
#define zenic_SerializableAllocation_h

#include "../Memory/HeapAllocator.h"

namespace zenic
{

class SerializableAllocation
{
public:

	enum { DefaultAlignment = 16 };

	SerializableAllocation(Allocator& allocator = HeapAllocator::allocator(), uint alignment = DefaultAlignment);

	SerializableAllocation& operator = (const SerializableAllocation& allocation);

	Allocator& allocator() const;
	uint alignment() const;

private:

	Allocator* m_allocator;
	uint m_alignment;
};

#include "SerializableAllocation.inl"

}

#endif
