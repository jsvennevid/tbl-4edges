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
#ifndef zenic_generic_TextureData_h
#define zenic_generic_TextureData_h

#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

namespace zenic
{

namespace generic
{

class TextureData : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;
public:

	enum // Flags
	{
		Volume	= 0x00000001,
		Alpha	= 0x00000002,
		RGB		= 0x00000004,
		CubeMap	= 0x00000008
	};

	enum // Side
	{
		NegativeX	= 0,
		PositiveX	= 1,
		NegativeY	= 2,
		PositiveY	= 3,
		NegativeZ	= 4,
		PositiveZ	= 5
	};

	class Pixel
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:
		f32 r;
		f32 g;
		f32 b;
		f32 a;
	};

	TextureData();
	virtual ~TextureData();

	virtual void serialize(Serializer& s);

	void setFlags(u32 flags);
	u32 flags() const;

	void setWidth(u32 width);
	u32 width() const;

	void setHeight(u32 height);
	u32 height() const;

	void setDepth(u32 depth);
	u32 depth() const;

	void setNumImages(u32 images);
	u32 numImages() const;

	void setPixels(uint index, DataPtr<Pixel> pixels);
	DataPtr<Pixel> pixels(u32 index) const;

private:

	class Image
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:
		DataPtr<Pixel> m_pixels;
	};

	u32 m_flags;
	u32 m_width;
	u32 m_height;
	u32 m_depth;

	DataPtr<Image> m_images;
};

#include "TextureData.inl"

}

}

#endif
