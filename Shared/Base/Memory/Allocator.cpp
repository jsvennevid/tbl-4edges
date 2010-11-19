#include "Allocator.h"

namespace zenic
{

Allocator* Allocator::ms_first = 0;

Allocator::~Allocator()
{
	// detach allocator from global list

	Allocator* curr = ms_first;
	Allocator* last = 0;

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

}
