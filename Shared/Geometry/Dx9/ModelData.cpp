#include "ModelData.h"

#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Base/Serialize/Serializer.h>

namespace zenic
{

namespace dx9
{

ZENIC_CREATE_SERIALIZABLE_FACTORY(ModelData,SERIALIZABLE_ID('D','X',' ','9'),SERIALIZABLE_ID('M','D','A','T'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(ModelData,Stream,SERIALIZABLE_ID('S','T','R','M'));

ModelData::~ModelData()
{
	for (uint i = 0, n = m_streams.count(); i < n; ++i)
		m_streams[i].data().free();
	m_streams.free();
}

void ModelData::serialize(Serializer& s)
{
	zenic::SerializableVersion version(1,factory());

	s << version;

	s.descriptor(SERIALIZER_NAME("m_type")) << m_type;
	s.descriptor(SERIALIZER_NAME("m_numVertices")) << m_numVertices;
	s.descriptor(SERIALIZER_NAME("m_numPrimitives")) << m_numPrimitives;

	s.beginStructure(m_streams,SERIALIZER_NAME("m_streams"));
	for (uint i = 0, n = m_streams.count(); i < n; ++i)
		m_streams[i].serialize(s);
	s.endStructure();
}

void ModelData::Stream::serialize(Serializer& s)
{
	s.descriptor(SERIALIZER_NAME("m_type")) << m_type;
	s.descriptor(SERIALIZER_NAME("m_method")) << m_method;
	s.descriptor(SERIALIZER_NAME("m_usage")) << m_usage;

	s.descriptor(SERIALIZER_NAME("m_index")) << m_index;
	s.descriptor(SERIALIZER_NAME("m_size")) << m_size;

	s.descriptor(SERIALIZER_NAME("m_data")) << m_data;
}

}

}
