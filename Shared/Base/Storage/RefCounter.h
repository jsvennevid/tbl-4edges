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
