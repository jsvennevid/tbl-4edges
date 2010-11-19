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
