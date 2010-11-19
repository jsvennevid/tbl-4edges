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
#include "Stream.h"

#include "../Debug/Assert.h"
#include "../Storage/Array.h"
#include "../Storage/String.h"

#include <stdio.h>
#include <string.h>

namespace zenic
{

Stream::~Stream()
{
}

bool Stream::copy(Stream& stream, int size)
{
	u8 buffer[CopySize];

	while (size)
	{
		int packet_size = size > CopySize ? CopySize : size;
		if (!read(buffer, packet_size))
			return false;
		if (!stream.write(buffer, packet_size))
			return false;
		size -= packet_size;
	}
	return true;
}

Stream& Stream::operator << (const void* pointer)
{
	*this << "0x" << hex << setw(sizeof(void*)) << setfill('0') << reinterpret_cast<u64>(pointer);
	return *this;
}

Stream& Stream::operator << (const c8* string)
{
	ZENIC_ASSERT(string);

	int length = static_cast<int>(::strlen(string));

	write(reinterpret_cast<const u8*>(string), length);

	return *this;
}

Stream& Stream::operator << (c8 value)
{
	write(reinterpret_cast<const u8*>(&value), 1);
	return *this;
}

Stream& Stream::operator << (u8 value)
{
	uintToAscii(value, m_hex, m_width, m_fillchar, false);
	resetManipulators();
	return *this;
}

Stream& Stream::operator << (s8 value)
{
	intToAscii(value);
	resetManipulators();
	return *this;
}

Stream& Stream::operator << (u16 value)
{
	uintToAscii(value, m_hex, m_width, m_fillchar, false);
	resetManipulators();
	return *this;
}

Stream& Stream::operator << (s16 value)
{
	intToAscii(value);
	resetManipulators();
	return *this;
}

Stream& Stream::operator << (u32 value)
{
	uintToAscii(value, m_hex, m_width, m_fillchar, false);
	resetManipulators();
	return *this;
}

Stream& Stream::operator << (s32 value)
{
	intToAscii(value);
	resetManipulators();
	return *this;
}

Stream& Stream::operator << (u64 value)
{
	uintToAscii(value, m_hex, m_width, m_fillchar, false);
	resetManipulators();
	return *this;
}

Stream& Stream::operator << (s64 value)
{
	intToAscii(value);
	resetManipulators();
	return *this;
}


Stream& Stream::operator << (f32 value)
{
	floatToAscii(value,m_precision);
	return *this;
}

f32 Stream::modf(f32 value, f32* intptr)
{
	return (value - (*intptr = floor(value)));
}

f32 Stream::floor(f32 value)
{
	f32 ret;

	if (value < 0.0)
	{
		if ((value - value) != 0.0f)
			ret = ((f32)(s64)value - 1.0f);
		else
			ret = value;
	}
	else
		ret = (f32)(s64)value;

	return ret;
}

void Stream::uintToAscii(u64 value, bool hex, uint width, char fillchar, bool negative)
{
	char buffer[64];
	char* curr = buffer;

	bool negated = false;

	if (negative && !m_hex && (s64(value) < 0))
	{
		negated = true;
		value = -s64(value);
	}

	do
	{
		static const char* map = "0123456789abcdef";
		uint c;

		if (hex)
		{
			c = uint(value&15);
			value >>= 4;
		}
		else
		{
			c = uint(value%10);
			value /= 10;
		}

		*curr++ = map[c];
	}
	while (value > 0);

	uint fillchars = (uint(curr-buffer) < width) ? width-uint(curr-buffer) : 0;

	for (uint i = 0; i < fillchars; ++i)
		*this << fillchar;

	if (negated)
		*this << '-';

	for (--curr; curr >= buffer; --curr)
		*this << *curr;
}

// TODO: implement this properly

void Stream::floatToAscii(f32 value, uint precision)
{
	f32 frac, n;
	uint i;

	if (value < .0f)
	{
		*this << '-';
		value = -value;
	}

	frac = modf(value, &n);

	// TODO: rounding modes?

	if (!precision)
		n += (frac + .5f);

	uintToAscii(u64(n), false, 0, 0, false);

	// TODO: how about processing the float directly through IEEE rules?
	// this kind of processing is affected by float inaccuracy

	// TODO: round the last decimal

	if (precision > 0)
	{
		for ( i = 0, *this << '.', frac *= 10.0f; i < precision; ++i, frac *= 10.0f)
			*this << c8((((int)frac % 10) + '0'));
	}

	resetManipulators();
}

// TODO: implement these differently on big-endian systems

bool Stream::write32BE(u32 value)
{
	u32 temp = ((value&0xff000000)>>24)|((value&0xff0000)>>8)|((value&0xff00)<<8)|((value&0xff)<<24);
	return write(&temp, sizeof(temp)) ? true : false;
}

bool Stream::read32BE(u32& value)
{
	u32 temp;
	if (!read(&temp, sizeof(temp)))
		return false;

	value = ((temp&0xff000000)>>24)|((temp&0xff0000)>>8)|((temp&0xff00)<<8)|((temp&0xff)<<24);

	return true;
}

Stream& Stream::operator >> (String& string)
{
	Array<c8> temp;

	// skip all leading whitespaces

	c8 c;
	while (read(&c,1))
	{
		if ((c == ' ')||(c == '\t')||(c == '\r')||(c == '\n'))
			continue;
		temp.pushBack(c);
		break;
	}

	// read all valid characters

	while (read(&c,1))
	{
		if ((c == ' ')||(c == '\t')||(c == '\r')||(c == '\n'))
			break;
		temp.pushBack(c);
	}

	temp.pushBack(0);
	const c8* s = temp.buffer();

	string = s;

	return *this;
}

u64 Stream::readUint()
{
	Array<c8> temp;

	// skip all leading whitespaces

	c8 c;
	while (read(&c,1))
	{
		if ((c == ' ')||(c == '\t')||(c == '\r')||(c == '\n'))
			continue;
		temp.pushBack(c);
		break;
	}

	// read all valid characters

	while (read(&c,1))
	{
		if ((c == ' ')||(c == '\t')||(c == '\r')||(c == '\n'))
			break;
		temp.pushBack(c);
	}

	temp.pushBack(0);
	const c8* s = temp.buffer();
	bool negative = false;
	if (*s == '-')
	{
		negative = true;
		++s;
	}

	u64 value = 0;
	for (;*s;++s)
	{
		if ((*s < '0')||(*s > '9'))
			break;

		value = (value * 10) + (*s-'0');
	}

	if (negative)
		value = -static_cast<s64>(value);
	return value;
}

f32 Stream::readFloat()
{
	Array<c8> temp;

	// skip all leading whitespaces

	c8 c;
	while (read(&c,1))
	{
		if ((c == ' ')||(c == '\t')||(c == '\r')||(c == '\n'))
			continue;
		temp.pushBack(c);
		break;
	}

	// read all valid characters

	while (read(&c,1))
	{
		if ((c == ' ')||(c == '\t')||(c == '\r')||(c == '\n'))
			break;
		temp.pushBack(c);
	}

	temp.pushBack(0);
	const c8* s = temp.buffer();

	// cheeeating, I know!
	f32 value;
	sscanf(s,"%f",&value);
	return value;
}

}
