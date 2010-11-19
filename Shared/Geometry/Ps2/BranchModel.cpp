///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


