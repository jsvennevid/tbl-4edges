inline Allocator::Allocator() : m_next(ms_first)
{
	ms_first = this;
}

inline Allocator* Allocator::next() const
{
	return m_next;
}

inline Allocator* Allocator::find(Handle handle)
{
	for (Allocator* current = ms_first; current; current = current->next())
	{
		if (current->handle() == handle)
			return current;
	}
	return 0;
}
