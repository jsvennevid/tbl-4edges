#include "HeapAllocator.h"

#include <malloc.h>
#include <stdio.h>

namespace zenic
{

HeapAllocator HeapAllocator::ms_allocator;

HeapAllocator::~HeapAllocator()
{
}

void* HeapAllocator::allocate(size_t size, size_t /*align*/)
{
#ifdef ZENIC_PS2
	return ::malloc(size);
#else
	return _aligned_malloc(size, 16);
#endif
}

void HeapAllocator::free(void* ptr)
{
#ifdef ZENIC_PS2
	printf("free ptr: 0x%x\n", (int)ptr);
	::free(ptr);
#else
	_aligned_free(ptr);
#endif
}

Allocator::Handle HeapAllocator::handle() const
{
	return 0;
}

}
