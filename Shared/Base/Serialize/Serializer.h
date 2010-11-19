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
#ifndef zenic_Serializer_h
#define zenic_Serializer_h

#include "Serializable.h"
#include "SerializableArray.h"

#include "../Memory/DataPtr.h"
#include "../Storage/Array.h"


namespace zenic
{
	class Stream;
	class SerializableVersion;
	class SerializableAllocation;
	class SerializableStructure;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

class Serializer
{
public:

	Serializer();
	virtual ~Serializer();

	virtual void reset();

	uint count() const;
	Serializable* operator [] (uint index) const;
	void add(Serializable* object);
	void remove(Serializable* object);

	Serializer& descriptor(const char* descriptor);

	template<class T> void operator << (T*& object);

	void operator << (SerializableVersion& version);
	void operator << (SerializableAllocation& allocation);
	template <class T> void operator << (SerializableArray<T>& array);

	void operator << (f32& v);
	void operator << (bool& v);

	void operator << (s8& v);
	void operator << (u8& v);
	void operator << (c8& v);

	void operator << (s16& v);
	void operator << (u16& v);

	void operator << (s32& v);
	void operator << (u32& v);

	void operator << (s64& v);
	void operator << (u64& v);

	template<class T> void operator << (DataPtr<T>& ptr); 

	void beginGroup(const char* name = 0, const char* type = 0);
	void endGroup();

	template<class T> void beginStructure(DataPtr<T>& ptr, const char* name = 0);
	void endStructure();

protected:

	enum
	{
		OBJECT_ALIGN = 1<<4,
		DATA_ALIGN = 1<<4
	};

	class Pointer
	{
	public:
		u8* m_objects;
		u32 m_count;
		Allocator* m_allocator;
	};

	virtual void pushGroup(const char* name, const char* type) = 0;
	virtual void popGroup() = 0;

	virtual void pushStructure(const char* name, SerializableStructure* structure, Pointer* ptr) = 0;
	virtual void popStructure() = 0;

	virtual void process(SerializableVersion& version) = 0;
	virtual void process(SerializableAllocation& allocation) = 0;
	virtual void process(const char* name, Serializable*& object) = 0;
	virtual void process(const char* name, Pointer* ptr, u32 elementSize) = 0;

	virtual void process(const char* name, f32& value) = 0;
	virtual void process(const char* name, bool& value) = 0;

	virtual void process(const char* name, s8& value) = 0;
	virtual void process(const char* name, u8& value) = 0;
	virtual void process(const char* name, c8& value) = 0;

	virtual void process(const char* name, s16& value) = 0;
	virtual void process(const char* name, u16& value) = 0;

	virtual void process(const char* name, s32& value) = 0;
	virtual void process(const char* name, u32& value) = 0;

	virtual void process(const char* name, s64& value) = 0;
	virtual void process(const char* name, u64& value) = 0;

private:

	Array<Serializable*> m_topObjects;
	const char* m_descriptor;
};

#include "Serializer.inl"

}

#define SERIALIZER_NAME(n) (n)

#endif
