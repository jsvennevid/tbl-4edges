///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
