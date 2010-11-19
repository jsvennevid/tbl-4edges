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

#ifndef zenic_BoneArray_h
#define zenic_BoneArray_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Bound.h"

#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>
#include <Shared/Base/Storage/String.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BoneArray : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;
public:

	class Bone
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:

		inline Bone() : m_target(0) {}

		void setTarget(Node* target);
		Node* target() const;

		void setBound(const Bound& bound);
		const Bound& bound() const;

		void setTransform(const Matrix4& transform);
		const Matrix4& transform() const;

		const String& name() const;
		void setName(const char* name);

		Matrix4 m_tempTransform;

	private:

		Node* m_target;

		Bound m_bound;
		Matrix4 m_transform;
		String m_name;
	};

	BoneArray();
	virtual ~BoneArray();

	void setBones( uint bones );
	uint bones() const;

	Bone& bone( uint index );

	virtual void serialize( Serializer& s );

private:

	DataPtr<Bone> m_bones;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "BoneArray.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
