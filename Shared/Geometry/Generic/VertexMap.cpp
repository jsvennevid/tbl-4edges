#include "VertexMap.h"

#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

namespace zenic
{

namespace generic
{

ZENIC_CREATE_SERIALIZABLE_FACTORY(VertexMap,SERIALIZABLE_ID('G','N','R','C'),SERIALIZABLE_ID('V','R','T','S'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(VertexMap,Index,SERIALIZABLE_ID('I','N','D','X'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(VertexMap,FloatData,SERIALIZABLE_ID('F','L','T','S'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(VertexMap,IntegerData,SERIALIZABLE_ID('I','N','T','S'));

VertexMap::~VertexMap()
{
	m_indices.free();
	m_floats.free();
	m_integers.free();
}

void VertexMap::serialize(Serializer& s)
{
	SerializableVersion version(1,factory());

	s << version;

	m_name.serialize(s,SERIALIZER_NAME("m_name"));

	s.descriptor(SERIALIZER_NAME("m_type")) << m_type;
	s.descriptor(SERIALIZER_NAME("m_usage")) << m_usage;
	s.descriptor(SERIALIZER_NAME("m_dimensions")) << m_dimensions;

	s.descriptor(SERIALIZER_NAME("m_next")) << m_next;

	s.beginStructure(m_indices,SERIALIZER_NAME("m_indices"));
	for (uint i = 0, n = m_indices.count(); i < n; ++i)
		s << m_indices[i].m_index;
	s.endStructure();

	s.beginStructure(m_floats,SERIALIZER_NAME("m_floats"));
	for (uint i = 0, n = m_floats.count(); i < n; ++i)
		s << m_floats[i].m_value;
	s.endStructure();

	s.beginStructure(m_integers,SERIALIZER_NAME("m_integers"));
	for (uint i = 0, n = m_integers.count(); i < n; ++i )
		s << m_integers[i].m_value;
	s.endStructure();
}

}

}
