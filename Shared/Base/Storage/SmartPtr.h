/*

Copyright (c) 2004-2006 Jesper Svennevid, Daniel Collin

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
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
