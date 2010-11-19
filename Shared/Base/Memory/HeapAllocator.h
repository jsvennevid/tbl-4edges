#ifndef zenic_HeapAllocator_h
#define zenic_HeapAllocator_h

#include "Allocator.h"

namespace zenic
{

class HeapAllocator : public Allocator
{
public:

	HeapAllocator();
	virtual ~HeapAllocator();

	virtual void* allocate(size_t size, size_t align = 0);
	virtual void free(void* ptr);

	virtual Handle handle() const;

	static Allocator& allocator();

private:

	static HeapAllocator ms_allocator;
};

#include "HeapAllocator.inl"

}

#endif
