#ifndef zenic_Array_h
#define zenic_Array_h

#include "../Debug/Assert.h"
#include "../Memory/DataPtr.h"

namespace zenic
{

template<class Tt, class Ta = HeapAllocator> class Array
{
public:

	Array() : m_count(0)
	{
	}

	~Array()
	{
		clear();
	}

	void clear()
	{
		m_buffer.free();
		m_count = 0;
	}

	void reset()
	{
		m_count = 0;
	}

	void pushBack(Tt element)
	{
		if (m_count == m_buffer.count())
		{
			uint newCount = matchingCount(m_count+1);
			m_buffer.resize(newCount, &Ta::allocator());
		}

		m_buffer[ m_count++ ] = element;
	}

	void popBack()
	{
		ZENIC_ASSERT(m_count);

		if (m_count > 0)
		{
			m_count--;
			m_buffer[m_count] = 0;
		}
	}

	void erase(uint index)
	{
		ZENIC_ASSERT(index < m_count);

		if (m_count > 0)
			m_buffer[index] = m_buffer[m_count-1];
		popBack();
	}

	u32 count() const { return m_count; }

	void resize(uint count)
	{
		reserve(count);
		m_count = count;
	}

	void reserve(uint count)
	{
		if (count > m_buffer.count())
			m_buffer.resize(count, &Ta::allocator());
	}

	void compress()
	{
		if (m_count < m_buffer.count())
			m_buffer.resize(m_count, &Ta::allocator());
	}

	Tt* buffer() { return m_buffer; }

	operator Tt* (void) const { return m_buffer; }

private:

	static u32 matchingCount(u32 count)
	{
		u32 bitCount;

		for (bitCount = 0; count > 0; count >>=1, ++bitCount);

		return 1<<bitCount;
	}

	u32 m_count;
	DataPtr<Tt> m_buffer;
};

}

#endif
