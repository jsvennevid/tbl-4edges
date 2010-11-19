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
