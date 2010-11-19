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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(ZENIC_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

Log logOut;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Log::Log() : m_offset(0)
{
	// to make sure that the string we send in ::flush() is null-terminated (note
	// that m_buffer is 1 character larger than BufferSize to permit this)
	m_buffer[BufferSize] = '\0';
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Log::~Log()
{
	flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Log::read(void* /*buffer*/, int /*size*/)
{
	return false;
}

int Log::write(const void* buffer, int size)
{
	while (size)
	{
		if (m_offset == BufferSize)
			flush();

		uint packet_size = size > (BufferSize - m_offset) ? BufferSize - m_offset : static_cast<uint>(size); 

		::memcpy(m_buffer + m_offset, buffer, packet_size);
		buffer = static_cast<const void*>(static_cast<const u8*>(buffer) + packet_size);
		size -= packet_size;
		m_offset += packet_size;
	}

	return true;
}

int Log::seek(SeekMode /*mode*/, int /*offset*/)
{
	return -1;
}

bool Log::flush()
{
	if (m_offset < BufferSize)
		(*this) << ends;

	if (m_offset > 0)
	{
#if defined(ZENIC_WIN32)
		OutputDebugString(m_buffer);
#elif defined(ZENIC_PS2)
		printf(m_buffer);
#endif
		m_offset = 0;
	}
	return true;
}

int Log::tell() const
{
	return -1;
}

}
