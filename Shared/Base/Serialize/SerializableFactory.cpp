#include "SerializableFactory.h"
#include "SerializableStructure.h"

namespace zenic
{

SerializableFactory* SerializableFactory::ms_first = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SerializableFactory::~SerializableFactory()
{
	// detach factory from global list

	SerializableFactory* curr = ms_first;
	SerializableFactory* last = 0;

	for (; curr ; last = curr, curr = curr->m_next)
	{
		if (this == curr)
		{
			if (last)
				last->m_next = curr->m_next;
			else
				ms_first = curr->m_next;
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SerializableFactory* SerializableFactory::find(Identifier host, Identifier type)
{
	for (SerializableFactory* factory = SerializableFactory::first(); factory; factory = factory->next())
	{
		if ((factory->host() == host) && (factory->type() == type))
			return factory;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SerializableStructure* SerializableFactory::findStructure(SerializableStructure::Identifier identifier) const
{
	for (SerializableStructure* structure = m_structures; structure; structure = structure->next())
	{
		if (structure->identifier() == identifier)
			return structure;
	}
	return 0;
}

}
