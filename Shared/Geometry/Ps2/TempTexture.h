///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_ps2_TempTexture_h
#define zenic_ps2_TempTexture_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Base/Memory/DataPtr.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TempTexture  : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;

public:
	enum ImageFormat
	{
		CT32	=0x00,
		CT24	= 0x01,
		CT16	= 0x02,
		T8		= 0x13,
		T4		= 0x14
	};

	TempTexture();
	virtual ~TempTexture();

	void serialize(Serializer& s);
	void addToChain(Dma& dmaChain);

	void init();

	void setImageData(u8* data, uint size) { m_imageData = DataPtr<u8>(data, size); }
	u8* imageData() { return m_imageData.objects(); }
	u32 imageSize() { return m_imageData.count(); }

	void setPalData(u8* data, uint size) { m_palData = DataPtr<u8>(data, size); }
	u8* palData() { return m_palData.objects(); }
	u32 palSize() { return m_palData.count(); }

	void setCallChain(u8* data, uint size) { m_callChain = DataPtr<u8>(data, size); }
	u8* callChain() { return m_callChain.objects(); }

	void setPalOffset(u32 offset) { m_palOffset = offset; }
	void setImageOffset(u32 offset) { m_imageOffset = offset; }


private:

	DataPtr<u8>	m_callChain;
	DataPtr<u8>	m_imageData;
	DataPtr<u8>	m_palData;
	
	u32	m_imageOffset;
	u32 m_palOffset;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif //zenic_ps2_Texture_h
