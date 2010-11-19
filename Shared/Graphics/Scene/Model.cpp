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

#include "Model.h"

#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Base/Serialize/Serializer.h>

#include <Shared/Geometry/ModelData.h>
#include <Shared/Graphics/Scene/BoneArray.h>

#include <Shared/Base/Io/Log.h>


#define ZENIC_UNCACHED_ACCELERATED(p) ((u8 *)((u32)p | 0x30000000))
#define ZENIC_UNCACHED(p) ((u8 *)((u32)p | 0x20000000))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

Model* Model::m_collectedModels[300];
Model** Model::m_collectedModelsList = 0;
uint Model::m_collectedModelsCount = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(Model, SERIALIZABLE_ID('Z','N','I','C'), SERIALIZABLE_ID('M','O','D','L'))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Model::~Model()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Model::resetCollectedModelCount()
{
	m_collectedModelsCount = 0;
	m_collectedModelsList = reinterpret_cast<Model**>(&m_collectedModels);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Model::update(float time)
{
	Node::update(time);
	ZENIC_ASSERT_DESC(m_collectedModelsCount < 300, "Maximum of 300 Models supported for now");
	m_collectedModelsList[m_collectedModelsCount++] = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Model::serialize(Serializer& s)
{
	SerializableVersion version(0xc0cac01a, factory());

	s << version;

	Node::serialize(s);

	s.descriptor("m_modelData") << m_modelData;
	s.descriptor("m_boneArray") << m_boneArray;
	s.descriptor("m_next") << m_next;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
