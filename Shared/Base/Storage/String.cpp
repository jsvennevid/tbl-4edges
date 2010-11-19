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

#include "String.h"

#include "../Serialize/Serializer.h"
#include "../Io/BufferStream.h"

#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String& String::operator = (const c8* in)
{
	ZENIC_ASSERT(in);
	size_t len = ::strlen(in);

	assign(in, u32(len+1));

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void String::assign(const c8* in, u32 inlen)
{
	m_string.free();

	u32 length = inlen;
	if (length < 1)
		length = 1;

	if (length > BufferSize)
		m_string.allocate(length);
	else
		m_string = DataPtr<c8>(m_buffer,length,0);

	::memcpy(m_string.objects(), in, length-1);
	m_string[length-1] = '\0';
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String String::subString(uint start, uint stop) const
{
	String temp;

	u32 len = length();

	if (stop > len)
		stop = len;

	if (start > stop)
		start = stop;
		
	temp.assign(m_string.objects() + start, (stop-start) + 1);
	return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int String::find(const char* match, uint start, uint stop) const
{
	ZENIC_ASSERT(match);
	size_t matchLength = ::strlen(match);

	size_t stringLength = length();

	for (; start < stop; ++start)
	{
		if (start >= stringLength)
			break;

	
		bool wasMatch = true;
		for (uint j = 0; j < matchLength; ++j)
		{
			if (match[j] != m_string[start+j])
			{
				wasMatch = false;
				break;
			}
		}

		if (wasMatch)
			return static_cast<int>(start);
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void String::serialize(Serializer& s, const char* name)
{
	s.descriptor(name) << m_string;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int String::compare(const char* s1, const char* s2, uint len)
{
	while (len--)
	{
		if (*s1 != *s2)
			return *s2-*s1;
		s1++;
		s2++;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int String::icompare(const char* s1, const char* s2, uint len)
{
	while (len--)
	{
		unsigned char c1 = static_cast<unsigned char>(*s1++),c2 = static_cast<unsigned char>(*s2++);

		if ((c1-'A') <= 'Z')
			c1 = (c1-'A') + 'a';

		if ((c2-'A') <= 'Z')
			c2 = (c2-'A') + 'a';

		if (c1 != c2)
			return c2-c1;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u32 String::quickHash() const
{
	u32 hash = 5381;
	const c8* str = contents();

	for (uint i = 0; i < length(); ++i)
	{
		u32 c = *str++;
		hash = hash * 33 ^ c;
	}

	return hash;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
