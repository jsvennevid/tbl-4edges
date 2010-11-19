#include "ModelData.h"

#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableStructure.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

namespace zenic
{

namespace generic
{

ZENIC_CREATE_SERIALIZABLE_FACTORY(ModelData,SERIALIZABLE_ID('G','N','R','C'),SERIALIZABLE_ID('M','D','A','T'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(ModelData,Index,SERIALIZABLE_ID('I','N','D','X'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(ModelData,Polygon,SERIALIZABLE_ID('P','O','L','Y'));

ModelData::~ModelData()
{
	m_polygons.free();
}

void ModelData::serialize(Serializer& s)
{
	SerializableVersion version(1,factory());
	s << version;

	zenic::ModelData::serialize(s);

	s.beginStructure(m_polygons,SERIALIZER_NAME("m_polygons"));
	for (uint i = 0, n = m_polygons.count(); i < n; ++i)
		m_polygons[i].serialize(s);
	s.endStructure();

	s.descriptor(SERIALIZER_NAME("m_vertices")) << m_vertexMap;
}

void ModelData::Polygon::serialize(Serializer& s)
{
	m_normal.serialize(s,SERIALIZER_NAME("m_normal"));

	s.beginStructure(m_indices,SERIALIZER_NAME("m_indices"));
	for (uint i = 0, n = m_indices.count(); i < n; ++i)
		s << m_indices[i].m_index;
	s.endStructure();
}

}

}
