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

#ifndef zenic_ps2_Texture_h
#define zenic_ps2_Texture_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Storage/Array.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Model;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Texture : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	enum { NumberOfMaps = 7 };

	class Parameters
	{
	public:
		Parameters();
		inline Parameters(u32 bufferWidth, u32 psm, u32 clutPsm, u32 width, u32 height);

		void serialize(Serializer& s, const char* name);

		void setBufferWidth(u32 bufferWidth);
		u32 bufferWidth() const;

		void setPsm(u32 psm);
		u32 psm() const;

		void setClutPsm(u32 clutPsm);
		u32 clutPsm() const;

		void setWidth(u32 width);
		u32 width() const;

		void setHeight(u32 height);
		u32 height() const;

	private:

		u32 m_bufferWidth;

		u32 m_psm;
		u32 m_clutPsm;

		u32 m_width;
		u32 m_height;
	};

	Texture();
	virtual ~Texture();

	virtual void serialize(Serializer& s);

	void setUploadParameters(const Parameters& upload);
	const Parameters& uploadParameters() const;

	void setDisplayParameters(const Parameters& display);
	const Parameters& displayParameters() const;

	void setMipMaps(u32 mipMaps);
	u32 mipMaps() const;

	void setPalette(DataPtr<u8> palette);
	DataPtr<u8>& palette();
	DataPtr<u8> palette() const;

	void setPalette2(DataPtr<u8> palette);
	const DataPtr<u8>& palette2() const;

	void setData(uint map, DataPtr<u8> data);
	DataPtr<u8> data(uint map) const;
	DataPtr<u8>& map(uint index);

	Array<Model*>& models();

private:

	Parameters m_uploadParameters;
	Parameters m_displayParameters;

	u32 m_mipMaps;

	DataPtr<u8> m_palette;
	DataPtr<u8> m_palette2;
	DataPtr<u8> m_data[NumberOfMaps];

	Array<Model*> m_models;
};

#include "Texture.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline DataPtr<u8>& Texture::map(uint index)
{
	return m_data[index];
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
