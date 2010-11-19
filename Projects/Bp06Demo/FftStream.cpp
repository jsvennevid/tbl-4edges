///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FftStream.h"
#include <Shared/Base/Io/Log.h>
#if defined(ZENIC_PS2)
#include <fileXio_rpc.h>
#include <loadfile.h>
#endif
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

FftStream* FftStream::s_instance = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FftStream::FftStream() : 
	m_kickData(0), 
	m_hihatData(0),
	m_bassData(0),
	m_speachData(0),
	m_offset(0) 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FftStream::create()
{
	//m_kickData = loadData("DATA/OUTPUT/KICK.FFT");
	//m_hihatData = loadData("DATA/OUTPUT/HIHAT.FFT");
	m_bassData = loadData("DATA/OUTPUT/BASS.FFT");
	m_speachData = loadData("DATA/OUTPUT/RUNNING.FFT");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FftStream::setTime(float time)
{
	m_offset = uint((time * 60.0f) * 16.0f); // 60 fps and 16 samples per frame
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FftStream::update(Renderer& renderer, Dma& dmaChain, bool visualize)
{
	if (visualize)
	{
		ZENIC_ASSERT(m_speachData);

		PostEffectHelper hlp(renderer, dmaChain);
		hlp.setRgba(127, 127, 127, 127); 

		for (uint i = 0; i < 16; ++i)
		{
			hlp.blit(192, 200, 192 + 10 * i, 200 + (m_speachData[m_offset + i]), false, false);
			hlp.flushRegisters();
		}
	}


	m_offset += 16;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8* FftStream::loadData(const char* filename)
{
	FileSystem& fileSystem = FileSystem::instance();
	FileStream stream;

	if (!fileSystem.open(stream, filename))
		return false;

	int fileSize = 267952; // hax0r

	u8* data = zenic_new u8[fileSize];
	stream.read(data, fileSize);
	stream.close();

	return data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FftStream& FftStream::instance()
{
	if (!s_instance)
		s_instance = zenic_new FftStream;

	ZENIC_ASSERT(s_instance);
	return *s_instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

