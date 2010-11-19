///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
