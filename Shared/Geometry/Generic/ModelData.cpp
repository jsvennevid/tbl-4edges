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
