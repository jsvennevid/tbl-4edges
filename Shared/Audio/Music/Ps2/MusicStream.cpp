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

#include "MusicStream.h"
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Io/FileSystem.h>
#include <kernel.h>
#include <loadfile.h>
#include <math.h>
#include <Shared/Base/Io/Log.h>

extern "C"
{
	#include <ps2snd.h>
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

bool MusicStream::m_hasLodadedIrx = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MusicStream::MusicStream() : m_isPlaying(false)
{
	//loadIrx();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MusicStream::MusicStream(const char* stream) : m_isPlaying(false)
{
	loadIrx();
	open(stream);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MusicStream::~MusicStream()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MusicStream::play()
{
	if (m_isPlaying)
		return;

	sndStreamPlay();

	m_isPlaying = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MusicStream::stop()
{
	sndStreamPause();
	sndStreamClose();
	m_isPlaying = false;
}

/*
void MusicStream::setPosition(uint pos)
{
//02:58 <jbit> FontPrintf(hud, font, "Stream Time:%3d:%02d.%02d  Block:%d\n", (int)(cur/(48000.0f*60.0f/28.0f)),
//             (int)(cur/(48000.0f/28.0f))%60, (int)(cur/(48000.0f/100.0f/28.0f))%100, cur);
}
*/

float MusicStream::setPosition(float time)
{
	if (!m_isPlaying)
		return time;

	//ZENIC_INFO("Setting Music stream to " << time);
	sndStreamSetPosition((int)((48000.0f / 28.0f) * time));
	return (floorf((48000 * time) / 28.0f) * 28.0f) / 48000.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MusicStream::open(const char* stream)
{
	char fullPath[256];

	FileSystem& fileSystem = FileSystem::instance();
	fileSystem.fullPath(fullPath, stream);

	ZENIC_INFO("Trying to load: pfs0:/DATA/OUTPUT/SAPART1.APM"); 

	if (sndStreamOpen("pfs0:/DATA/OUTPUT/MUSIC.APM", SD_VOICE(0,22) | (SD_VOICE(0,23)<<16), STREAM_STEREO | STREAM_END_CLOSE, 0x6000, 1024) < 0)
	//if (sndStreamOpen(fullPath, SD_VOICE(0,22) | (SD_VOICE(0,23)<<16), STREAM_STEREO | STREAM_END_CLOSE, 0x6000, 1024) < 0)
	{
		ZENIC_ERROR("Unable to open stream " << stream);
		SleepThread();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MusicStream::loadIrx()
{
	if (m_hasLodadedIrx)
		return;

	int ret = SifLoadModule("host:data/irx/freesd.irx", 0, NULL);
	if (ret < 0)
	{
		ZENIC_ERROR("XXXXX failed to load host:irx/freesd.irx: " << ret);
		SleepThread();
	}

	ret = SifLoadModule("host:data/irx/ps2snd.irx", 0, NULL);
	if (ret < 0)
	{
		ZENIC_ERROR("XXXXX failed to load host:irx/ps2snd.irx: " << ret);
		SleepThread();
	}

	if (SdInit(0) < 0)
	{
		ZENIC_ERROR("XXXXX failed to init sound.");
		SleepThread();
	}

	SdSetParam(0 | SD_PARAM_MVOLL, 0x3fff);
	SdSetParam(0 | SD_PARAM_MVOLR, 0x3fff);
	SdSetParam(1 | SD_PARAM_MVOLL, 0x3fff);
	SdSetParam(1 | SD_PARAM_MVOLR, 0x3fff);

	m_hasLodadedIrx = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

