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

#ifndef zenic_GlenzNode_h
#define zenic_GlenzNode_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Node.h"
#include <Shared/Base/Math/Vector3.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class GlenzNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GlenzNode : public Node
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	enum
	{
		MaxCollectedNodes = 300
	};

	GlenzNode();
	virtual ~GlenzNode();
	
	static void resetCollected();
	static uint collectedCount();
	static const GlenzNode** collectedNodes();

	void update(float time);
	void serialize(Serializer& s);

	Vector3 m_angles;
	Vector3 m_anglesSpeed;

	static uint s_collectedCount;
	static GlenzNode* s_collectedNodes[MaxCollectedNodes];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void GlenzNode::resetCollected()
{
	s_collectedCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint GlenzNode::collectedCount()
{
	return s_collectedCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const GlenzNode** GlenzNode::collectedNodes()
{
	return reinterpret_cast<const GlenzNode**>(&s_collectedNodes);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif



