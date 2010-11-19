#ifndef zenic_Allocator_h
#define zenic_Allocator_h

#include "../Types.h"
#include "New.h"

#include <new>

namespace zenic
{

class Allocator
{
public:

	Allocator();
	virtual ~Allocator();

	typedef u32 Handle;

	virtual void* allocate(size_t size, size_t align = 0) = 0;
	virtual void free(void* ptr) = 0;

	virtual Handle handle() const = 0;

	static Allocator* find(Handle handle);

	Allocator* next() const;
	static Allocator* first();

private:

	Allocator* m_next;
	static Allocator* ms_first;
};

#include "Allocator.inl"

#define ALLOCATOR_HANDLE(a,b,c,d) (((a)<<24)|((b)<<16)|((c)<<8)|(d))

}

#endif
