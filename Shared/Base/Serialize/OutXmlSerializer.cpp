#include "OutXmlSerializer.h"

#include "SerializableFactory.h"
#include "SerializableStructure.h"
#include "SerializableVersion.h"


#include "../Io/BufferStream.h"
#include "../Io/Log.h"

#include "../Debug/Assert.h"

#include "../Storage/Base64.h"
#include "../Storage/Xml/XmlDocument.h"
#include "../Storage/Xml/XmlElement.h"
#include "../Storage/Xml/XmlText.h"
#include "../Storage/Xml/XmlComment.h"
#include "../Storage/Xml/XmlDeclaration.h"

#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OutXmlSerializer::OutXmlSerializer()
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OutXmlSerializer::~OutXmlSerializer()
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::reset()
{
	Serializer::reset();

	m_objects.clear();
	m_current = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool OutXmlSerializer::save(Stream& stream)
{
	XmlDocument doc;

	XmlDeclaration* decl = zenic_new XmlDeclaration;
	decl->setValue("xml");
	decl->setAttribute("version","1.0");
	decl->setAttribute("encoding","ISO-8859-1");
	doc.addChild(decl);

	XmlComment* comment = new XmlComment;
	comment->setValue(" Cell XML Object Graph ");
	doc.addChild(comment);

	// add initial objects

	for (uint i = 0, n = count(); i < n; ++i)
	{
		Serializable* initial = (*this)[i];
		uint j,m;
		for (j = 0, m = m_objects.count(); j < m; ++j)
		{
			if (m_objects[j] == initial)
				break;
		}

		if (j == m_objects.count())
			m_objects.pushBack(initial);
	}

	// build xml tree

	XmlElement* root = new XmlElement;
	root->setValue("zenic");

	for (uint i = 0; i < m_objects.count(); ++i)
	{
		Serializable* object = m_objects[i];
		ZENIC_ASSERT(object);

		m_current = 0;
		m_currentIndex = i;
		object->serialize(*this);

		ZENIC_ASSERT(m_current);
		root->addChild(m_current);
	}

	doc.addChild(root);

	// write tree to stream
	if (!doc.save(stream))
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::pushGroup(const char* name, const char* type)
{
	XmlElement* group = new XmlElement;
	group->setValue("group");

	if (name)
		group->setAttribute("name",name);

	if (type)
		group->setAttribute("type",type);

	m_stack.pushBack(m_current);
	m_current->addChild(group);
	m_current = group;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::popGroup()
{
	ZENIC_ASSERT(!(m_current->value()).compare("group"));
	ZENIC_ASSERT(m_stack.count());

	m_current = m_stack[m_stack.count()-1];
	m_stack.popBack();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::pushStructure(const char* name, SerializableStructure* structure, Pointer* ptr)
{
	ZENIC_ASSERT(structure);

	XmlElement* st = new XmlElement;
	st->setValue("struct");

	if (name)
		st->setAttribute("name",name);

	SerializableFactory* factory = structure->factory();
	{
		BufferStream temp;
		temp << hex << setw(8) << setfill('0') << factory->host();
		st->setAttribute("host",temp.string());
	}
	{
		BufferStream temp;
		temp << hex << setw(8) << setfill('0') << factory->type();
		st->setAttribute("type",temp.string());
	}
	{
		BufferStream temp;
		temp << hex << setw(8) << setfill('0') << structure->identifier();
		st->setAttribute("id",temp.string());
	}

	if (ptr->m_allocator && (ptr->m_allocator != &HeapAllocator::allocator()))
	{
		BufferStream temp;
		temp << hex << setfill('0') << setw(8) << ptr->m_allocator->handle();
		st->setAttribute("allocator",temp.string());
	}
	
	{
		BufferStream temp;
		temp << ptr->m_count;
		st->setAttribute("count",temp.string());
	}

	m_stack.pushBack(m_current);
	m_current->addChild(st);
	m_current = st;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::popStructure()
{
	ZENIC_ASSERT(!(m_current->value()).compare("struct"));
	ZENIC_ASSERT(m_stack.count());

	m_current = m_stack[m_stack.count()-1];
	m_stack.popBack();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(SerializableVersion& version)
{
	if (!m_current)
	{
		ZENIC_ASSERT(version.factory());

		XmlElement* current = zenic_new XmlElement;
		current->setValue("object");
		
		{
			BufferStream temp;
			temp << m_currentIndex;
			current->setAttribute("id",temp.string());
		}

		{
			BufferStream temp;
			temp << hex << setw(8) << setfill('0') << version.factory()->host();
			current->setAttribute("host",temp.string());
		}

		{
			BufferStream temp;
			temp << hex << setw(8) << setfill('0') << version.factory()->type();
			current->setAttribute("type",temp.string());
		}

		m_current = current;
	}

	XmlElement* v = zenic_new XmlElement;
	v->setValue("version");

	XmlText* text = zenic_new XmlText;
	BufferStream temp;
	temp << version.version();
	text->setValue(temp.string());
	v->addChild(text);

	m_current->addChild(v);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(SerializableAllocation& allocation)
{
	m_allocation = allocation;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, Serializable*& object)
{
	XmlElement* reference = new XmlElement;
	reference->setValue("ref");

	if (name)
		reference->setAttribute("name",name);

	XmlText* text = new XmlText;

	if (object)
	{
		// insert into list if needed

		uint i;

		for (i = 0; i < m_objects.count(); ++i)
		{
			if (m_objects[i] == object)
				break;
		}

		if (i == m_objects.count())
			m_objects.pushBack(object);

		BufferStream temp;
		temp << i;
		text->setValue(temp.string());
	}
	else
		text->setValue("null");

	reference->addChild(text);

	ZENIC_ASSERT(m_current);
	m_current->addChild(reference);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, Pointer* ptr, u32 elementSize)
{
	XmlElement* data = new XmlElement;
	data->setValue("data");

	if (name)
		data->setAttribute("name",name);

	if ((&m_allocation.allocator()) != (&HeapAllocator::allocator()))
	{
		BufferStream temp;
		temp << hex << setfill('0') << setw(8) << ptr->m_allocator->handle();

		data->setAttribute("allocator",temp.string());
	}

	if (m_allocation.alignment() != SerializableAllocation::DefaultAlignment)
	{
		BufferStream temp;
		temp << m_allocation.alignment();
		data->setAttribute("align",temp.string());
	}

	if (ptr->m_count > 0)
	{
		BufferStream temp;
		temp << ptr->m_count;
		data->setAttribute("count",temp.string());
	}

	{
		BufferStream temp;
		temp << elementSize;
		data->setAttribute("size",temp.string());
	}

	{
		BufferStream temp;
		Base64::encode(temp,ptr->m_objects,elementSize * ptr->m_count);

		if (temp.buffer().count() > 0)
		{
			XmlText* text = zenic_new XmlText;
			text->setValue(temp.string());
			data->addChild(text);
		}
	}	

	ZENIC_ASSERT(m_current);
	m_current->addChild(data);

	m_allocation = SerializableAllocation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, f32& value)
{
	BufferStream string;
	string << value;
	process(name, "f32", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, bool& value)
{
	BufferStream string;
	string << value;
	process(name, "bool", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, s8& value)
{
	BufferStream string;
	string << value;
	process(name, "s8", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, u8& value)
{
	BufferStream string;
	string << value;
	process(name, "u8", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, c8& value)
{
	BufferStream string;
	string << value;
	process(name, "c8", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, s16& value)
{
	BufferStream string;
	string << value;
	process(name, "s16", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, u16& value)
{
	BufferStream string;
	string << value;
	process(name, "u16", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, s32& value)
{
	BufferStream string;
	string << value;
	process(name, "s32", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, u32& value)
{
	BufferStream string;
	string << value;
	process(name, "u32", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, s64& value)
{
	BufferStream string;
	string << value;
	process(name, "s64", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, u64& value)
{
	BufferStream string;
	string << value;
	process(name, "u64", string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OutXmlSerializer::process(const char* name, const char* valueType, BufferStream& value)
{
	XmlElement* variable = zenic_new XmlElement;
	variable->setValue(valueType);

	if (name)
		variable->setAttribute("name",name);

	XmlText* text = zenic_new XmlText;
	text->setValue(value.string());
	variable->addChild(text);

	ZENIC_ASSERT(m_current);
	m_current->addChild(variable);
}

}
