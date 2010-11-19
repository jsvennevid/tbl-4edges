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

#include "TempTexture.h"
#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

#include <Shared/Base/Io/File.h>
#include <Shared/Base/Memory/New.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/DmaSimulator.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>

#ifdef ZENIC_WIN32
#pragma warning(disable : 4311)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(TempTexture, SERIALIZABLE_ID('P','S','2',' '), SERIALIZABLE_ID('T','M','P','T'));


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TempTexture::TempTexture() : m_imageOffset(0), m_palOffset(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TempTexture::~TempTexture()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TempTexture::serialize(Serializer& s)
{
	zenic::SerializableVersion version(1, factory());

	s << version;

	s.descriptor(SERIALIZER_NAME("m_callChain")) << m_callChain;
	s.descriptor(SERIALIZER_NAME("m_imageData")) << m_imageData;
	s.descriptor(SERIALIZER_NAME("m_palData")) << m_palData;

	s.descriptor(SERIALIZER_NAME("m_imageOffset")) << m_imageOffset;
	s.descriptor(SERIALIZER_NAME("m_palOffset")) << m_palOffset;

	// TODO: Only when texture is in use

	ZENIC_INFO("TempTexture::serialize");

	init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TempTexture::init()
{
	u8* callChainTemp = 0;
	u8* callChain = m_callChain.objects();
	
	callChainTemp = callChain + m_imageOffset;
	((u32*)callChainTemp)[1] = u32(m_imageData.objects());

	callChainTemp = callChain + m_palOffset;
	((u32*)callChainTemp)[1] = u32(m_palData.objects());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TempTexture::addToChain(Dma& dmaChain)
{
	dmaChain.addSrcCallTag(m_callChain.objects());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}

#ifdef ZENIC_WIN32
#pragma warning(default : 4311)
#endif

