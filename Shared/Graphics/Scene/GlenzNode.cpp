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

#include "GlenzNode.h"

#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Base/Serialize/Serializer.h>

#include <Shared/Base/Io/Log.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(GlenzNode, SERIALIZABLE_ID('Z','N','I','C'), SERIALIZABLE_ID('G','L','N','Z'))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint m_seed = 93186752;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float floatRandom()  
{
	static uint a = 1588635695, m = 4294967291U, q = 2, r = 1117695901;
	m_seed = a*(m_seed % q) - r*(m_seed / q);
	return ((float)m_seed / (float)m);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint GlenzNode::s_collectedCount = 0;
GlenzNode* GlenzNode::s_collectedNodes[MaxCollectedNodes];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GlenzNode::GlenzNode()
{
	m_angles.x = floatRandom();
	m_angles.y = floatRandom();
	m_angles.z = floatRandom();

	m_anglesSpeed.x = floatRandom();
	m_anglesSpeed.y = floatRandom();
	m_anglesSpeed.z = floatRandom();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GlenzNode::~GlenzNode()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlenzNode::update(float /*time*//*, float deltaTime*/)
{
	float deltaTime = 1.0f / 60.0f;	// TODO: Fix me badly
	Vector3 pos = transform().position();

	Matrix4 mx;
	Matrix4 my;
	Matrix4 mz;

	mx.makeXrotation(m_angles.x);
	my.makeYrotation(m_angles.y);
	mz.makeZrotation(m_angles.z);

	m_angles.x += m_anglesSpeed.x * deltaTime;
	m_angles.y += m_anglesSpeed.y * deltaTime;
	m_angles.z += m_anglesSpeed.z * deltaTime;

	Matrix4 matrix = mx * my * mz;
	matrix.setPosition(pos);

	setTransform(matrix);

	ZENIC_ASSERT_DESC(s_collectedCount < MaxCollectedNodes, "Maximum of 300 GlenzNodes supported for now");
	s_collectedNodes[s_collectedCount++] = this;

	// No need to update node here as we rotate these nodes ourself
	//Node::update(time);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlenzNode::serialize(Serializer& s)
{
	SerializableVersion version(1, factory());

	s << version;

	Node::serialize(s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}



