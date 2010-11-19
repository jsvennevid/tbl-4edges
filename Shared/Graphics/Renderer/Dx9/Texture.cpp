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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Texture.h"

#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

namespace win32
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(Texture, SERIALIZABLE_ID('D','X','9','R'), SERIALIZABLE_ID('T','X','T','R'));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Texture::setSurfaces( u32 surfaces )
{
	if( surfaces < m_surfaces.count() )
	{
		for( uint i = surfaces; i < m_surfaces.count(); ++i )
			m_surfaces[i].data().free();
	}

	m_surfaces.resize( surfaces );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Texture::serialize( Serializer& s )
{
	SerializableVersion version(1,factory());
	s << version;

	s.descriptor(SERIALIZER_NAME("m_width")) << m_width;
	s.descriptor(SERIALIZER_NAME("m_height")) << m_height;
	s.descriptor(SERIALIZER_NAME("m_depth")) << m_depth;
	s.descriptor(SERIALIZER_NAME("m_format")) << m_format;
	s.descriptor(SERIALIZER_NAME("m_properties")) << m_properties;

	s.beginStructure(m_surfaces,SERIALIZER_NAME("m_surfaces"));
	for( uint i = 0; i < m_surfaces.count(); ++i )
		m_surfaces[i].serialize(s);
	s.endStructure();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Texture::Surface::serialize( Serializer& s )
{
	s.descriptor(SERIALIZER_NAME("m_pitch")) << m_pitch;
	s.descriptor(SERIALIZER_NAME("m_data")) << m_data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

}
