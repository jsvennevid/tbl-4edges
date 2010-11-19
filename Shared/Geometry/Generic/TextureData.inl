inline TextureData::TextureData() : m_flags(0), m_width(0), m_height(0), m_depth(0)
{
}

inline void TextureData::setFlags(u32 flags)
{
	m_flags = flags;
}

inline u32 TextureData::flags() const
{
	return m_flags;
}

inline void TextureData::setWidth(u32 width)
{
	m_width = width;
}

inline u32 TextureData::width() const
{
	return m_width;
}

inline void TextureData::setHeight(u32 height)
{
	m_height = height;
}

inline u32 TextureData::height() const
{
	return m_height;
}


inline void TextureData::setDepth(u32 depth)
{
	m_depth = depth;
}

inline u32 TextureData::depth() const
{
	return m_depth;
}

inline void TextureData::setNumImages(u32 images)
{
	m_images.resize(images);
}

inline u32 TextureData::numImages() const
{
	return m_images.count();
}

inline void TextureData::setPixels(uint index, DataPtr<Pixel> pixels)
{
	ZENIC_ASSERT(index < m_images.count());
	m_images[index].m_pixels = pixels;
}

inline DataPtr<TextureData::Pixel> TextureData::pixels(u32 index) const
{
	ZENIC_ASSERT(index < m_images.count());
	return m_images[index].m_pixels;
}
