#ifndef zenic_generic_Texture_h
#define zenic_generic_Texture_h

#include <Shared/Base/Storage/String.h>
#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>

namespace zenic
{

namespace generic
{
	class TextureData;
}

}

namespace zenic
{

namespace generic
{

class Texture : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	Texture();
	virtual ~Texture();

	virtual void serialize(Serializer& s);

	void setPath(const String& path);
	const String& path() const;

	void setData(TextureData* data);
	TextureData* data() const;

private:

	String m_path;
	TextureData* m_data;
};

#include "Texture.inl"

}

}

#endif
