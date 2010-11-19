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
#include "SerializableManager.h"

#include "Serializable.h"

#include "../Debug/Assert.h"

namespace zenic
{

SerializableManager::~SerializableManager()
{
	release();

	for (uint i = 0; i < m_pools.count(); ++i)
		m_pools[i].free();
	m_pools.free();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SerializableManager::insert(Serializable* object, bool owned)
{
	ZENIC_ASSERT(object);
	ZENIC_ASSERT(!object->m_nextManaged);

	if (owned)
	{
		object->m_nextManaged = m_firstOwned;
		m_firstOwned = object;
	}
	else
	{
		object->m_nextManaged = m_first;
		m_first = object;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SerializableManager::remove(Serializable* object)
{
	ZENIC_ASSERT(object);

	Serializable* curr = m_firstOwned;
	Serializable* last = 0;
	bool owned = true;

	while (curr && (curr != object))
	{
		last = curr;
		curr = curr->m_nextManaged;
	}

	if (!curr)
	{
		owned = false;

		curr = m_first;
		last = 0;

		while (curr && (curr != object))
		{
			last = curr;
			curr = curr->m_nextManaged;
		}
	}

	ZENIC_ASSERT(curr);

	if (!last)
	{
		if (!owned)
			m_firstOwned = curr->m_nextManaged;
		else
			m_first = curr->m_nextManaged;
	}
	else
		last->m_nextManaged = curr->m_nextManaged;

	if (!owned)
		HeapAllocator::allocator().free(object);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SerializableManager::release()
{
	while (m_firstOwned)
		delete m_firstOwned;
	while (m_first)
		delete m_first;
}

}
