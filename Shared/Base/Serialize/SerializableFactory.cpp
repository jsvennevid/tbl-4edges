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
