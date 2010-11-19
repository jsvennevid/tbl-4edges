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

#ifndef zenic_win32_Texture_h
#define zenic_win32_Texture_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// forwards
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

namespace win32
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Texture : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;
public:

	class Surface
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:

		Surface();

		void setPitch( u32 pitch );
		u32 pitch() const;

		void setData( DataPtr<u8> data );
		DataPtr<u8> data() const;

	private:
		u32 m_pitch;
		DataPtr<u8> m_data;
	};

	enum // Properties
	{
		Compressed		= 0x00000001,
		Float			= 0x00000002,

		Cubemap			= 0x00000010,
		Volume			= 0x00000020,
		Mipmaps			= 0x00000040,

		PositiveX		= 0x04000000,
		NegativeX		= 0x08000000,
		PositiveY		= 0x10000000,
		NegativeY		= 0x20000000,
		PositiveZ		= 0x40000000,
		NegativeZ		= 0x80000000
	};

	Texture();
	virtual ~Texture();

	void setWidth( u32 width );
	u32 width() const;

	void setHeight( u32 height );
	u32 height() const;

	void setDepth( u32 depth );
	u32 depth() const;

	void setFormat( u32 format );
	u32 format() const;

	void setProperties( u32 properties );
	u32 properties() const;

	void setSurfaces( u32 surfaces );
	u32 surfaces() const;

	Surface& surface( u32 index );

	virtual void serialize( Serializer& s );

private:

	u32 m_width;
	u32 m_height;
	u32 m_depth;
	u32 m_format;
	u32 m_properties;

	DataPtr<Surface> m_surfaces;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Texture.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

}

#endif
