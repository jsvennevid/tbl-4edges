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
