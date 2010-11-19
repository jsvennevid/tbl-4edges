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

#ifndef zenic_String_h
#define zenic_String_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../Memory/DataPtr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Serializer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class String
{
public:

	String();
	String(const c8* in);
	String(const c8* in, u32 length);
	String(const String& in);
	~String();

	String& operator = (const c8* in);
	String& operator = (const String& in);

	String subString(uint start = 0, uint stop = uint(-1)) const;

	int compare(const String& string) const;
	int compare(uint pos, uint len, const String& string) const;

	int icompare(const String& string) const;
	int icompare(uint pos, uint len, const String& string) const;

	int compare(uint pos1, uint len1, const String& string, uint pos2, uint len2) const;
	int icompare(uint pos1, uint len1, const String& string, uint pos2, uint len2) const;

	static int compare(const char* s1, const char* s2, uint len);
	static int icompare(const char* s1, const char* s2, uint len);

	int find(const c8* match, uint start = 0, uint stop = uint(-1)) const;

	u32 quickHash() const;

	operator const c8*() const;
	const c8* str() const;

	const c8* contents() const;
	u32 length() const;
	bool empty() const;

	void serialize(Serializer& s, const char* name);

private:

	enum { BufferSize = 16 };

	void assign(const c8* in, uint length);

	DataPtr<c8> m_string;
	c8 m_buffer[BufferSize];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "String.inl"

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
