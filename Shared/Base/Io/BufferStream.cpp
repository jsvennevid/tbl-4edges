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
#include "BufferStream.h"

#include <string.h>

namespace zenic
{

BufferStream::~BufferStream()
{
	m_buffer.free();
}

int BufferStream::read(void* buffer, int size)
{
	if (uint(m_offset + size) > m_buffer.count())
		size -= (m_offset + size)-m_buffer.count();

	::memcpy(buffer, m_buffer.objects() + m_offset, static_cast<size_t>(size));

	m_offset += uint(size);

	return size;
}

int BufferStream::write(const void* buffer, int size)
{
	// buffer small writes to avoid constant resizing of memory buffer

	int packet_size = 0;

	const u32* bla = (const u32*)buffer;

	int bla2 = 0;

	for (uint i = 0; i < (uint)size / 4; ++i)
	{
		if (*bla == 0xcdcdcdcd)
			bla2 = 1;

		++bla;
	}

	if ((m_internalOffset+size) <= BufferSize)
	{
		packet_size = size > (BufferSize-m_internalOffset) ? (BufferSize-m_internalOffset) : size;

		::memcpy(m_internalBuffer + m_internalOffset, buffer, packet_size);

		m_internalOffset += packet_size;
		buffer = static_cast<const void*>(static_cast<const u8*>(buffer) + packet_size);
		size -= packet_size;
	}

	if (size > 0)
	{
		if (uint(m_offset + m_internalOffset + size) > m_buffer.count())
		{
			m_buffer.resize(static_cast<u32>(m_offset + m_internalOffset + size));
			flush();
		}

		::memcpy(m_buffer.objects() + m_internalOffset + m_offset, buffer, static_cast<size_t>(size));
		m_offset = static_cast<uint>(m_offset + size);
	}

	return size + packet_size;
}

int BufferStream::seek(SeekMode mode, int offset)
{
	int lastOffset = m_offset;
	flush();

	switch (mode)
	{
		case Start:
		{
			if ((offset < 0) || (uint(offset) > m_buffer.count()))
				return false;

			m_offset = static_cast<uint>(offset);
		}
		break;

		case Current:
		{
			if (((m_offset+offset) < 0) || (uint(m_offset+offset) > m_buffer.count()))
				return false;

			m_offset = static_cast<uint>(m_offset + offset);
		}
		break;

		case End:
		{
			if (((m_buffer.count()+offset) < 0) || ((m_buffer.count()+offset) > m_buffer.count()))
				return false;

			m_offset = static_cast<uint>(m_buffer.count() + offset);
		}
		break;
	}

	return lastOffset;
}

int BufferStream::tell() const
{
	return m_offset;
}

bool BufferStream::flush()
{
	if (m_internalOffset > 0)
	{
		if (uint(m_offset + m_internalOffset) > m_buffer.count())
			m_buffer.resize(m_offset + m_internalOffset + 1);

		::memcpy(m_buffer.objects() + m_offset, m_internalBuffer, m_internalOffset);
		m_buffer[m_buffer.count()-1] = 0;

		m_offset += m_internalOffset;
		m_internalOffset = 0;
	}

	return true;
}

}
