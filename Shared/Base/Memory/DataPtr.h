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
#ifndef zenic_DataPtr_h
#define zenic_DataPtr_h

#include "HeapAllocator.h"
#include "../Debug/Assert.h"

namespace zenic
{

template<class T> class DataPtr
{
public:

	DataPtr()
	{
		m_objects = 0;
		m_count = 0;
		m_allocator = 0;
	}

	DataPtr(T* objects, u32 count, Allocator* allocator = &HeapAllocator::allocator())
	{
		m_objects = objects;
		m_count = count;
		m_allocator = allocator;
	}

	DataPtr(const DataPtr& t)
	{
		m_objects = t.m_objects;
		m_count = t.m_count;
		m_allocator = t.m_allocator;
	}

	void allocate(u32 count, Allocator* allocator = &HeapAllocator::allocator())
	{
		ZENIC_ASSERT(allocator);

		T* objects = 0;

		if (count > 0)
		{
			objects = static_cast<T*>(allocator->allocate(sizeof(T) * count));
			ZENIC_ASSERT(objects);

			for (u32 i = 0; i < count; ++i)
				new (objects+i) T;
		}

		m_objects = objects;
		m_count = count;
		m_allocator = allocator;
	}

	void free()
	{
		T* objects = m_objects;

		if (objects)
		{
			for (u32 i = m_count; i > 0; --i)
				objects[i-1].~T();

			if (m_allocator)
				m_allocator->free(objects);
		}

		m_objects = 0;
		m_count = 0;
		m_allocator = 0;
	}

	DataPtr clone() const
	{
		DataPtr clone;

		ZENIC_ASSERT(m_allocator);

		if (m_objects && m_allocator)
		{
			clone.allocate(m_count, m_allocator);

			T* src = m_objects;
			T* dest = clone;

			for (u32 i = 0, n = m_count; i < n; ++i)
				(*dest++) = (*src++);
		}

		return clone;
	}

	void copy(const DataPtr& ptr)
	{
		const T* src = ptr;
		T* dest = m_objects;

		if (ptr.count() < m_count)
		{
			for (u32 i = 0, n = ptr.count(); i < n; ++i)
				(*dest++) = (*src++);
		}
		else
		{
			for (u32 i = 0, n = m_count; i < n; ++i)
				(*dest++) = (*src++);
		}
	}

	void resize(u32 newCount, Allocator* allocator = &HeapAllocator::allocator())
	{
		DataPtr newData;

		ZENIC_ASSERT(allocator);

		if ((newCount != m_count) || ((newCount > 0) && (allocator != m_allocator)))
		{
			newData.allocate(newCount, allocator);
			newData.copy(*this);
			free();
			*this = newData;
		}
	}

	void reserve(u32 newCount, Allocator* allocator = &HeapAllocator::allocator())
	{
		if ((newCount != m_count) || ((newCount > 0) && (allocator != m_allocator)))
		{
			free();
			allocate(newCount, allocator);
		}
	}

	void swap(DataPtr& t)
	{
		DataPtr temp = t;
		t = *this;
		*this = temp;
	}

	DataPtr& operator = (const DataPtr& t)
	{
		m_objects = t.m_objects;
		m_count = t.m_count;
		m_allocator = t.m_allocator;

		return *this;
	}

	T* operator-> (void) const { return m_objects; }
	operator T* (void) const { return m_objects; }

	T* objects() const { return m_objects; }
	u32 count() const { return m_count; }

	Allocator* allocator() const { return m_allocator; }

private:

	T* m_objects;
	u32 m_count;
	Allocator* m_allocator;
};

}

#endif
