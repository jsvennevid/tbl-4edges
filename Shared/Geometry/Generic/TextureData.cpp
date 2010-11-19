#include "TextureData.h"

#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

namespace zenic
{

namespace generic
{

ZENIC_CREATE_SERIALIZABLE_FACTORY(TextureData,SERIALIZABLE_ID('G','N','R','C'),SERIALIZABLE_ID('T','D','A','T'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(TextureData,Image,SERIALIZABLE_ID('I','M','G','E'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(TextureData,Pixel,SERIALIZABLE_ID('P','I','X','L'));

TextureData::~TextureData()
{
	for (uint i = 0, n = m_images.count(); i < n; ++i)
		m_images[i].m_pixels.free();
	m_images.free();
}

void TextureData::serialize(Serializer& s)
{
	zenic::SerializableVersion version(1,factory());

	s << version;

	s.descriptor(SERIALIZER_NAME("m_flags")) << m_flags;
	s.descriptor(SERIALIZER_NAME("m_width")) << m_width;
	s.descriptor(SERIALIZER_NAME("m_height")) << m_height;
	s.descriptor(SERIALIZER_NAME("m_depth")) << m_depth;

	s.beginStructure(m_images,SERIALIZER_NAME("m_images"));
	for (uint i = 0, n = m_images.count(); i < n; ++i)
		m_images[i].serialize(s);
	s.endStructure();
}

void TextureData::Image::serialize(Serializer& s)
{
	s.beginStructure(m_pixels,SERIALIZER_NAME("m_pixels"));
	for (uint i = 0, n = m_pixels.count(); i < n; ++i)
	{
		Pixel& pixel = m_pixels[i];

		s << pixel.r;
		s << pixel.g;
		s << pixel.b;
		s << pixel.a;
	}
	s.endStructure();
}

}

}
