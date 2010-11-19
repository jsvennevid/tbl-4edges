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

#include "Texture.h"

#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(Texture, SERIALIZABLE_ID('P','S','2',' '), SERIALIZABLE_ID('T','X','T','R'));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture::~Texture()
{
	m_palette.free();

	for (uint i = 0; i < NumberOfMaps; ++i)
		m_data[i].free();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Texture::serialize(Serializer& s)
{
	zenic::SerializableVersion version(1, factory());

	s << version;

	m_uploadParameters.serialize(s,SERIALIZER_NAME("m_uploadParameters"));
	m_displayParameters.serialize(s,SERIALIZER_NAME("m_displayParameters"));

	s.descriptor(SERIALIZER_NAME("m_mipMaps")) << m_mipMaps;

	s.descriptor(SERIALIZER_NAME("m_palette")) << m_palette;
	s.descriptor(SERIALIZER_NAME("m_palette2")) << m_palette2;

	s.beginGroup(SERIALIZER_NAME("m_data"));

	for (uint i = 0; i < m_mipMaps; ++i)
		s << m_data[i];

	s.endGroup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Texture::Parameters::serialize(Serializer& s, const char* name)
{
	s.beginGroup(name,SERIALIZER_NAME("zenic::ps2::Texture::Parameters"));

	s.descriptor(SERIALIZER_NAME("m_bufferWidth")) << m_bufferWidth;

	s.descriptor(SERIALIZER_NAME("m_psm")) << m_psm;
	s.descriptor(SERIALIZER_NAME("m_clutPsm")) << m_clutPsm;

	s.descriptor(SERIALIZER_NAME("m_width")) << m_width;
	s.descriptor(SERIALIZER_NAME("m_height")) << m_height;

	s.endGroup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
