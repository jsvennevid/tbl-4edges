#ifndef zenic_RefCounter_h
#define zenic_RefCounter_h

namespace zenic
{


template <class T> class RefCounter;

template <class T> class RefCounted
{
protected:
	T* m_object;
	RefCounter* m_counter;
	RefCounted* m_prev;
	RefCounted* m_next;
};

template <class T> class RefCounter
{
public:
	RefCounter(T* object) : m_object(object)
	{
		ZENIC_ASSERT_DESC(object,"RefCounter cannot track null pointer");

		m_weakPointers.m_next = m_weakPointers.m_prev = &m_weakPointers;
		m_smartPointers.m_next = m_smartPointers.m_prev = &m_smartPointers;
	}

	void addSmart(const RefCounted& pointer)
	{
		ZENIC_ASSERT(pointer.m_counter == this);
		pointer.m_prev = m_smartPointers.m_prev;
		pointer.m_next = &m_smartPointers;
		m_smartPointers.m_prev->m_next = &pointer;
		m_smartPointers.m_prev = &pointer;
	}

	void addWeak(const RefCounted& pointer)
	{
		ZENIC_ASSERT(pointer.m_counter == this);
		pointer.m_prev = m_weakPointers.m_prev;
		pointer.m_next = &m_weakPointers;
		m_weakPointers.m_prev->m_next = &pointer;
		m_weakPointers.m_prev = &pointer;
	}

	void releaseSmart(const RefCounted& pointer)
	{
		ZENIC_ASSERT(pointer.m_counter == this);
		ZENIC_ASSERT(m_smartPointers.m_next != &m_smartPointers);

		pointer.m_prev->m_next = pointer->m_next;
		pointer.m_next->m_prev = pointer->m_prev;

		if (m_smartPointers.m_next == &m_smartPointers)
			delete this;
	}

	void releaseWeak(const RefCounted& pointer)
	{
		ZENIC_ASSERT(pointer.m_counter == this);
		ZENIC_ASSERT(m_weakPointers.m_next != &m_weakPointers);

		pointer.m_prev->m_next = pointer->m_next;
		pointer.m_next->m_prev = pointer->m_prev;
	}
private:
	~RefCounter()
	{
		ZENIC_ASSERT(m_smartPointers.m_next == &m_smartPointers,"Counter still has smart references");

		// clear weak pointers
		for (RefCounted* i = m_weakPointers.m_next, n = &m_weakPointers, next; i != n; i = next)
		{
			next = i->m_next;

			i->m_object = 0;
			i->m_counter = 0;

			i->m_prev = 0;
			i->m_next = 0;
		}
	}

	T* m_object;

	RefCounted m_smartPointers;
	RefCounted m_weakPointers;
};

}

#endif
