inline HeapAllocator::HeapAllocator()
{
}

inline Allocator& HeapAllocator::allocator()
{
	return ms_allocator;
}
