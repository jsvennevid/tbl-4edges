#ifndef zenic_WeakPtr_h
#define zenic_WeakPtr_h

#include "RefCounter.h"

namespace zenic
{

template <class T> class WeakPtr : public RefCounted
{
public:
	WeakPtr(const SmartPtr& pointer) : m_object(pointer.m_object), m_counter(pointer.m_counter), m_prev(0), m_next(0)
	{
		if (m_object)
		{
			ZENIC_ASSERT(m_counter);
			m_counter->addWeak(this);
		}
	}

	~WeakPtr()
	{
		if (m_object)
		{
			ZENIC_ASSERT(m_counter);
			m_counter->releaseWeak(this);
		}
	}

	WeakPtr<T>& operator = (const WeakPtr& pointer)
	{
		if (m_object != pointer.m_object)
		{
			if (m_object)
			{
				ZENIC_ASSERT(m_counter);
				m_counter->releaseWeak(this);
				m_counter = 0;
			}

			m_object = pointer.m_object;
			m_counter = pointer.m_counter;

			if (m_object)
			{
				ZENIC_ASSERT(m_counter);
				m_counter->addWeak(this);
			}
		}
	}

	operator T* (void) const { return m_object; }
private:
};

}

#endif
