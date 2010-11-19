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
