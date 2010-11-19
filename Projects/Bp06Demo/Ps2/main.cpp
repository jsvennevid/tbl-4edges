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

#include <Shared/Graphics/Renderer/Ps2/ShaderSystem.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/DebugRenderer.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>
#include <Shared/Hardware/Ps2/Vu0.h>
#include <Shared/Hardware/Ps2/MasterChain.h>

#include <Shared/Base/Io/File.h>
#include <Shared/Base/Io/FileStream.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Geometry/Ps2/ModelData.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Graphics/Scene/AnimationController.h>
#include <Shared/Input/Ps2/Pad.h>
#include <Shared/Base/Io/FileSystem.h>

#ifdef ZENIC_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#endif

#ifndef ZENIC_WIN32
#include <Shared/Audio/Music/MusicStream.h>
#endif

#include <string.h>

#include "../Parts/BranchStatuePart.h"
#include "../Parts/StartPart.h"
#include "../Parts/MountainsPart.h"
#include "../Parts/CityScapePart.h"
#include "../Parts/LogoPart.h"
#include "../Parts/ParkingHousePart.h"
#include "../Parts/LoadingPart.h"
#include "../Parts/SpeachPart1.h"
#include "../Parts/SpeachPart2.h"
#include "../Parts/TunnelPart.h"
#include "../Parts/EndPart.h"
#include "../Parts/BlobPart.h"
#include "../Parts/SaPart1.h"
#include "../Parts/SaPart2.h"
#include "../Parts/SaPart3.h"
#include "../Parts/SaPart4.h"
#include "../Parts/SaPart5.h"
#include "../Parts/SaPart6.h"
#include "../SyncPoints.h"
#include "../FftStream.h"
#include "../PadCamera.h"

//#define ONE_PART
//#define NO_MUSIC
//#define NO_HDD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		void demoMain();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ZENIC_WIN32
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int /*argc*/, char* /*argv[]*/ )
#endif
{   
	zenic::ps2::demoMain();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

Pad pad;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void demoMain()
{
	Renderer* renderer = zenic_new ps2::Renderer(false);
	ZENIC_ASSERT(renderer);

	float deltaTime = 1.0f / 60.0f;
	float time = 0.0f;

	if (!renderer)
		return;

#if defined(ZENIC_PS2)
#ifndef ONE_PART
#else
	//FileSystem::instance().setIoMode(FileSystem::Host);
#endif
	FileSystem::instance().setIoMode(FileSystem::Hdd);
	//FileSystem::instance().setIoMode(FileSystem::Host);
#endif

	Part* parts[14];

	renderer->create();
	renderer->setClearColor(127, 127, 127);

	renderer->init();

	LoadingPart* loadingPart = zenic_new LoadingPart(*renderer, MasterChain::instance()->chain());
	loadingPart->create();
	loadingPart->update();

	FftStream::instance().create();

	SyncPoints syncPoints;
	SaPart::loadSaObjects();

	// Alloc all parts parts

	for (uint i = 0; i < 14; ++i)
		parts[i] = 0;

#if defined(ZENIC_PS2)
#ifndef NO_MUSIC
	MusicStream* stream = zenic_new MusicStream("DATA/OUTPUT/SAPART1.APM");
#else
	MusicStream dummy;
	MusicStream* stream = &dummy;
#endif
#endif

#ifndef ONE_PART

	parts[0] = zenic_new StartPart(*renderer, MasterChain::instance()->chain());
	parts[1] = zenic_new MountainsPart(*renderer, MasterChain::instance()->chain());
	parts[2] = zenic_new SpeachPart1(*renderer, MasterChain::instance()->chain());;
	parts[3] = zenic_new SaPart1(*renderer, MasterChain::instance()->chain());
	parts[4] = zenic_new BlobPart(*renderer, MasterChain::instance()->chain());
	parts[5] = zenic_new SaPart2(*renderer, MasterChain::instance()->chain());
	parts[6] = zenic_new CityScapePart(*renderer, MasterChain::instance()->chain());
	parts[7] = zenic_new TunnelPart(*renderer, MasterChain::instance()->chain());
	parts[8] = zenic_new SpeachPart2(*renderer, MasterChain::instance()->chain());;
	parts[9] = zenic_new SaPart3(*renderer, MasterChain::instance()->chain());
	parts[10] = zenic_new ParkingHousePart(*renderer, MasterChain::instance()->chain());
	parts[11] = zenic_new EndPart(*renderer, MasterChain::instance()->chain());

	for (uint i = 0; i < 14; ++i)
	{
		if (parts[i])
			parts[i]->create();
	}


	// Ugly but what the hell

	((SaPart*)parts[8])->setScene(((SaPart*)parts[2])->scene());

	
/*
	u8* tempBuffer = zenic_new u8[8 * ((512 * 512) / 2)];
	ZENIC_ASSERT(tempBuffer);

	for (uint i = 0; i < (8 * ((512 * 512) / 2)); ++i)
		tempBuffer[i] = i;
*/
#else

	parts[0] = zenic_new ParkingHousePart(*renderer, MasterChain::instance()->chain());
	//parts[0] = zenic_new SpeachPart1(*renderer, MasterChain::instance()->chain());;
	//parts[0] = zenic_new StartPart(*renderer, MasterChain::instance()->chain());
	//parts[0] = zenic_new TunnelPart(*renderer, MasterChain::instance()->chain());
	//parts[0] = zenic_new CityScapePart(*renderer, MasterChain::instance()->chain());
	//parts[0] = zenic_new MountainsPart(*renderer, MasterChain::instance()->chain());
	parts[0]->create();

#endif

	while (time < 1.0f)
	{
		loadingPart->update(1.0f, time);
		renderer->flip();
		time += deltaTime;
	}


	// Fade down the Loading screen before the "real" demo start

	while (time < 1.0f)
	{
		loadingPart->update(1.0f, time);
		renderer->flip();
		time += deltaTime;
	}

	// Do real demo
	//MusicStream stream;
#ifndef ZENIC_FINAL
	pad.create(0, 0);
	PadCamera padCamera(pad);
	parts[0]->setPadCamera(&padCamera);
	

#endif

#if defined(ZENIC_PS2)
#ifndef NO_MUSIC
	stream->play();
#endif
#endif

	uint prevPartId = 0;

	time = 0.0f;
	FftStream::instance().setTime(0);

	//renderer->setRenderFrames(true);

	while (1) 
	{
#ifndef ONE_PART
		uint partId = syncPoints.partId();

		if (prevPartId != partId)
		{
			if (parts[prevPartId])
				parts[prevPartId]->reset();

			prevPartId = partId;
		}

		if (parts[partId])
			parts[partId]->update(time - syncPoints.partStartTime(), deltaTime);
#else
		parts[0]->update(time, deltaTime);
#endif

#if !defined(ZENIC_FINAL) && defined(ZENIC_PS2)
		time = syncPoints.update(pad, *stream, time);
#else
		time = syncPoints.update(time);
#endif


#if !defined(ZENIC_FINAL) && defined(ZENIC_PS2)
		pad.update();
		padCamera.update();

		if (pad.status().triangle)
		{
			renderer->screenShot("host:frames/screenshot.tga");
		}

		if (pad.status().select)
		{
			stream->stop();
		}
#endif

#ifndef NO_MUSIC
#if defined(ZENIC_PS2)
		if (time > 278.0f)
		{
			stream->stop();
		}
#endif
#endif

		FftStream::instance().update(*renderer, MasterChain::instance()->chain());

		renderer->flip();
		time += deltaTime;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

