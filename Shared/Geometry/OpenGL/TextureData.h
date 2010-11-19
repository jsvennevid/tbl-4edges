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

#ifndef zenic_opengl_TextureData_h
#define zenic_opengl_TextureData_h

#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	TextureData();
	~TextureData();

	virtual void serialize(Serializer& s);

	void setFlags(u32 flags);
	u32 flags() const;

	void setWidth(u16 width);
	u16 width() const;

	void setHeight(u16 height);
	u16 height() const;

	void setImageData(const DataPtr<u8>& data);
	const DataPtr<u8>& imageData() const;

private:

	u32 m_flags;
	u16 m_width;
	u16 m_height;

	DataPtr<u8> m_imageData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "TextureData.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif
