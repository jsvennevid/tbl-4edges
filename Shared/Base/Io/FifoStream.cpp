#include "FifoStream.h"

#include <string.h>

namespace zenic
{

FifoStream::FifoStream() : m_reading(0), m_writing(0)
{
}

FifoStream::~FifoStream()
{
	m_buffer.free();
}

int FifoStream::read(void* buffer, int size)
{
	int maxRead = 0;
	if (m_reading <= m_writing)
		maxRead = m_writing-m_reading;
	else
		maxRead = (m_writing+m_buffer.count())-m_reading;

	if (maxRead < size)
		size = maxRead;

	int actualRead = size;

	char* currentBuffer = static_cast<char*>(buffer);
	while (size > 0)
	{
		int currentRead = size;

		if (uint(m_reading + currentRead) > m_buffer.count())
			currentRead = m_buffer.count() - m_reading;

		::memcpy(currentBuffer, m_buffer.objects() + m_reading, currentRead);
		::memset(m_buffer.objects() + m_reading, 0xdd, currentRead);

		m_reading += currentRead;
		if (uint(m_reading) == m_buffer.count())
			m_reading = 0;

		currentBuffer += currentRead;
		size -= currentRead;
	}

	return actualRead;
}

int FifoStream::write(const void* buffer, int size)
{
	int maxWrite = 0;
	if (m_writing <= m_reading)
		maxWrite = (m_reading-m_writing)-1;
	else
		maxWrite = ((m_reading+m_buffer.count())-m_writing)-1;

	if (maxWrite < size)
	{
		DataPtr<u8> newData;

		// TODO: resize 2^x to allow for efficient performance

		newData.allocate(m_buffer.count() + (size-maxWrite));

		if (m_reading <= m_writing)
		{
			::memcpy(newData.objects(), &m_buffer.objects()[m_reading], m_writing-m_reading);
			m_writing -= m_reading;
			m_reading = 0;
		}
		else
		{
			// INCORRECT!
			::memcpy(newData.objects(), m_buffer.objects() + m_reading, m_buffer.count()-m_reading);
			::memcpy(newData.objects() + (m_buffer.count()-m_reading), m_buffer.objects(), m_writing);
			m_writing = m_reading-m_writing;
			m_reading = 0;
		}

		m_buffer.free();
		m_buffer = newData;
	}

	int actualWrite = size;
	const char* currentBuffer = static_cast<const char*>(buffer);
	while (size > 0)
	{
		int currentWrite = size;
		if (uint(m_writing + currentWrite) > m_buffer.count())
			currentWrite = m_buffer.count() - m_writing;

		::memcpy(m_buffer.objects() + m_writing, currentBuffer, currentWrite);

		m_writing += currentWrite;
		if (m_buffer.count() == uint(m_writing))
			m_writing = 0;

		currentBuffer += currentWrite;
		size -= currentWrite;
	}

	return actualWrite;
}

int FifoStream::seek(SeekMode /*mode*/, int /*offset*/)
{
	return -1;
}

bool FifoStream::flush()
{
	return true;
}

int FifoStream::tell() const
{
	return -1;
}

}
