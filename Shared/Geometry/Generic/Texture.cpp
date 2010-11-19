#include "Texture.h"

#include "TextureData.h"

#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

namespace zenic
{

namespace generic
{

ZENIC_CREATE_SERIALIZABLE_FACTORY(Texture,SERIALIZABLE_ID('G','N','R','C'),SERIALIZABLE_ID('T','X','T','R'));

Texture::~Texture()
{
}

void Texture::serialize(Serializer& s)
{
	zenic::SerializableVersion version(1,factory());

	s << version;

	m_path.serialize(s,SERIALIZER_NAME("m_path"));
	s.descriptor(SERIALIZER_NAME("m_data")) << m_data;
}

}

}
