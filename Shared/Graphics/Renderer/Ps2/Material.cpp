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

#include "Material.h"
#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Geometry/Ps2/Texture.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(Material, SERIALIZABLE_ID('P','S','2',' '), SERIALIZABLE_ID('M','A','T','R'))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Material::Material() : 
	m_color(0, 0, 0, 0), 
	m_glow(false), 
	m_additive(false), 
	m_transValue(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Material::~Material()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Material::serialize(Serializer& s)
{
	SerializableVersion version(2, factory());

	s << version;
	m_texture.serialize(s);

	s.descriptor(SERIALIZER_NAME("m_color.r")) << m_color.r;
	s.descriptor(SERIALIZER_NAME("m_color.g")) << m_color.g;
	s.descriptor(SERIALIZER_NAME("m_color.b")) << m_color.b;
	s.descriptor(SERIALIZER_NAME("m_color.a")) << m_color.a;

	s.descriptor(SERIALIZER_NAME("m_glow")) << m_glow;
	s.descriptor(SERIALIZER_NAME("m_additive")) << m_additive;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Material::execute(Dma& /*dmaChain*/)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

