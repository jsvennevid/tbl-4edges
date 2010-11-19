#include "Serializer.h"

namespace zenic
{

Serializer::~Serializer()
{
}

void Serializer::reset()
{
	m_topObjects.reset();
}

}
