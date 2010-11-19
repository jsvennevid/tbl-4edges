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

#include "BranchModel.h"
#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Io/Log.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(BranchModel, SERIALIZABLE_ID('P','S','2',' '), SERIALIZABLE_ID('B','R','M','D'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(BranchModel, Vertex, SERIALIZABLE_ID('V','E','R','T'));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BranchModel::BranchModel() : 
	m_scale(0.0f, 0.0f, 0.0f),	
	m_offset(0.0f, 0.0f, 0.0f) 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BranchModel::~BranchModel()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BranchModel::serialize(Serializer& s)
{
	SerializableVersion version(1, factory());

	s << version;

	s.beginStructure(m_curves, SERIALIZER_NAME("m_curves"));
	for (uint i = 0, n = m_curves.count(); i < n; ++i)
		m_curves[i].serialize(s);
	s.endStructure();

	s.descriptor(SERIALIZER_NAME("m_curvesCount")) << m_curvesCount;
	
	s.descriptor(SERIALIZER_NAME("m_scale.x")) << m_scale.x;
	s.descriptor(SERIALIZER_NAME("m_scale.y")) << m_scale.y;
	s.descriptor(SERIALIZER_NAME("m_scale.z")) << m_scale.z;

	s.descriptor(SERIALIZER_NAME("m_offset.x")) << m_offset.x;
	s.descriptor(SERIALIZER_NAME("m_offset.y")) << m_offset.y;
	s.descriptor(SERIALIZER_NAME("m_offset.z")) << m_offset.z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BranchModel::Vertex::serialize(Serializer& s)
{
	s.descriptor(SERIALIZER_NAME("x")) << x;
	s.descriptor(SERIALIZER_NAME("y")) << y;
	s.descriptor(SERIALIZER_NAME("z")) << z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}


