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

#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Base/Serialize/Serializer.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(ModelData, SERIALIZABLE_ID('O','G','L',' '), SERIALIZABLE_ID('M','D','A','T'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(ModelData, PolygonList, SERIALIZABLE_ID('P','L','S','T'));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ModelData::ModelData() : m_type(Rigid)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ModelData::~ModelData()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ModelData::serialize(Serializer& s)
{
	zenic::SerializableVersion version(1, factory());

	s << version;
	s << m_type;

	s.beginStructure(m_polygons, SERIALIZER_NAME("m_polygons"));

	for (uint i = 0, n = m_polygons.count(); i < n; ++i)
		m_polygons[i].serialize(s);

	s.endStructure();

	s.descriptor(SERIALIZER_NAME("m_material")) << m_material;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ModelData::PolygonList::serialize(Serializer& s)
{
	s.descriptor(SERIALIZER_NAME("m_polygonType")) << m_polygonType;
	s.descriptor(SERIALIZER_NAME("m_vertexFormat")) << m_vertexFormat;
	s.descriptor(SERIALIZER_NAME("m_vertexStream")) << m_vertexStream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

