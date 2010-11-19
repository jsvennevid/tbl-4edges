#include "New.h"

#include "../Io/Log.h"
#include "HeapAllocator.h"

void* operator new(size_t sz, const char* filename, uint line)
{
	void* p = zenic::HeapAllocator::allocator().allocate(sz);
	zenic::logOut << "new :: 0x" << p << "(" << u32(sz) << " bytes) :: " << filename << ", line " << line << zenic::endl;
	return p;
}
