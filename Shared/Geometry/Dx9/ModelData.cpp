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
