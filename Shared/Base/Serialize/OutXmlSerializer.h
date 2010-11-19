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
#ifndef zenic_OutXmlSerializer_h
#define zenic_OutXmlSerializer_h

#include "OutSerializer.h"

#include "SerializableAllocation.h"

#include "../Io/Stream.h"
#include "../Storage/Array.h"

namespace zenic
{
	class Stream;
	class BufferStream;
	class SerializableFactory;
	class XmlNode;
}

namespace zenic
{

class OutXmlSerializer : public OutSerializer
{
public:

	OutXmlSerializer();
	virtual ~OutXmlSerializer();

	virtual void reset();
	virtual bool save(Stream& stream);

protected:

	virtual void pushGroup(const char* name, const char* type);
	virtual void popGroup();

	virtual void pushStructure(const char* name, SerializableStructure* structure, Pointer* ptr);
	virtual void popStructure();

	virtual void process(SerializableVersion& version);
	virtual void process(SerializableAllocation& allocation);
	virtual void process(const char* name, Serializable*& object);
	virtual void process(const char* name, Pointer* ptr, u32 elementSize);

	virtual void process(const char* name, f32& value);
	virtual void process(const char* name, bool& value);

	virtual void process(const char* name, s8& value);
	virtual void process(const char* name, u8& value);
	virtual void process(const char* name, c8& value);

	virtual void process(const char* name, s16& value);
	virtual void process(const char* name, u16& value);

	virtual void process(const char* name, s32& value);
	virtual void process(const char* name, u32& value);

	virtual void process(const char* name, s64& value);
	virtual void process(const char* name, u64& value);

private:

	void process(const char* name, const char* valueType, BufferStream& value);

	Array<Serializable*> m_objects;
	Array<XmlNode*> m_stack;
	XmlNode* m_current;
	uint m_currentIndex;
	SerializableAllocation m_allocation;
};

}

#endif
