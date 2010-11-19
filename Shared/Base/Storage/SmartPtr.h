#ifndef zenic_SmartPtr_h
#define zenic_SmartPtr_h

#include "RefCounter.h"

namespace zenic
{

template <class T> class SmartPtr : public RefCounted
{
public:
	SmartPtr(T* object = 0) : m_object(object), m_counter(0), m_prev(0), m_next(0)
	{
		if (object)
		{
			m_counter = new RefCounter(object);
			m_counter->addSmart(this);
		}
	}

	SmartPtr(const SmartPtr& pointer) : m_object(pointer.m_object), m_counter(pointer.m_counter), m_prev(0), m_next(0)
	{
		if (m_object)
		{
			ZENIC_ASSERT(m_counter);
			m_counter->addSmart(this);
		}
	}

	~SmartPtr()
	{
		if (m_object)
		{
			ZENIC_ASSERT(m_counter);
			m_counter->releaseSmart(this);
		}
	}

	SmartPtr<T>& operator = (T* object)
	{
		ZENIC_ASSERT_DESC(!object || (object != m_object),"Assignment of already referenced pointer");
		if (m_counter)
		{
			m_counter->releaseSmart(this);
			m_object = 0;
			m_counter = 0;
		}

		if (object)
		{
			m_object = object;
			m_counter = new RefCounter(object);
			m_counter->addSmart(this);
		}
	}

	SmartPtr<T>& operator = (const SmartPtr& pointer)
	{
		if (m_object != pointer.m_object)
		{
			if (m_object)
			{
				ZENIC_ASSERT(m_counter);
				m_counter->releaseSmart(this);
				m_counter = 0;
			}

			m_object = pointer.m_object;
			m_counter = pointer.m_counter;

			if (m_object)
			{
				ZENIC_ASSERT(m_counter);
				m_counter->addSmart(this);
			}
		}
	}

	operator T* (void) const { return m_object; }
private:
};

}

#endif
