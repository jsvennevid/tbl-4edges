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

#include "SyncPoints.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Io/Log.h>
#include <string.h>
#ifndef ZENIC_FINAL
#include <Shared/Input/Ps2/Pad.h>
#include <Shared/Audio/Music/Ps2/MusicStream.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SyncPoints::SyncPoints() : m_currentPart(0)
{
	reloadFile();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float SyncPoints::update(Pad& pad, MusicStream& stream, float time)
{
#ifndef ZENIC_FINAL
	
	const Pad::Buttons& buttons = pad.status();

	if (buttons.l1)
	{
		--m_currentPart;

		if (m_currentPart < 0)
			m_currentPart = 0;

		ZENIC_INFO("Part " << m_currentPart);
#ifndef PS2_EMU
		return stream.setPosition(0.0f/*partStartTime()*/);
#endif
	}

	if (buttons.r1)
	{
		if (m_parts.count() > ((uint)m_currentPart + 1))
		{
			++m_currentPart;
			ZENIC_INFO("Part " << m_currentPart);
#ifndef PS2_EMU
			return stream.setPosition(0.0f/*partStartTime()*/);
#endif
		}
	}

	if (buttons.start)
	{
		reloadFile();
		ZENIC_INFO("Reloading syncfile");
	}

#endif

	return update(time);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float SyncPoints::update(float time)
{
	if (m_parts.count() > ((uint)m_currentPart + 1))
	{
		if (time >= m_parts.buffer()[m_currentPart+1]->startTime)
		{
			++m_currentPart;
			ZENIC_INFO("Switched part");
		}
	}

	return time;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float SyncPoints::partStartTime()
{
	return m_parts.buffer()[m_currentPart]->startTime;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint SyncPoints::partId()
{
	return m_parts.buffer()[m_currentPart]->id;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SyncPoints::reloadFile()
{
#if defined(ZENIC_PS2)
	File::IoMode oldMode = File::ioMode();

	//FileSystem& fileSystem = FileSystem::instance();

	File::setMode(File::Ioman);

	FileStream stream;
	if (!stream.open("host:DATA/OUTPUT/SYNC.TXT", FileStream::Read))
	{
		ZENIC_WARNING("Failed to load host:DATA/OUTPUT/SYNC.TXT");
		File::setMode(oldMode);
		return;
	}

	//if (!fileSystem.open(stream, "DATA/OUTPUT/SYNC.TXT"))
	//	return;

	// parse the file
	// Clear the existing stuff first

	m_parts.clear();

	Part* currentPart = 0;
	uint line = 0;

	String currentId;

	uint timeOut = 0;

	while (1)
	{
		++timeOut;

		if (timeOut == 50)
			break;

		stream >> currentId;
		ZENIC_INFO(currentId);

		// Check if we have a new part

		if (!stricmp("Part", currentId.contents()))
		{
			// if we already had a part, add it to the list

			if (currentPart)
				m_parts.pushBack(currentPart);

			currentPart = zenic_new Part;

			// Expect id, startTime, endTime	

			stream >> currentPart->id >> currentPart->startTime; 	

		} 
		else if (!stricmp("Custom", currentId.contents()))
		{
			if (!currentPart)
			{
				ZENIC_ERROR("Trying to add SyncPoint without part at line " << line);
				return;
			}
		}
		else if (!stricmp("CameraSwitch", currentId.contents()))
		{
			if (!currentPart)
			{
				ZENIC_ERROR("Trying to add SyncPoint without part at line " << line);
				return;
			}

			
			SyncPoint sync;
			sync.type = CameraChange;
			SwitchCamera* cameraSwitch = (SwitchCamera*)&sync.data;

			stream >> cameraSwitch->time >> cameraSwitch->time;

			currentPart->syncPoints.pushBack(sync);
		}

		else if (!stricmp("end", currentId.contents()))
		{
			break;
		}
		
		++line;
	}

	if (currentPart)
		m_parts.pushBack(currentPart);

#ifndef ZENIC_FINAL

	ZENIC_INFO("Total number of Parts " << m_parts.count());

	Part** parts = m_parts.buffer();

	for (uint i = 0; i < m_parts.count(); ++i)
	{
		Part* part = parts[i];
		ZENIC_INFO("Part " << i << " id " << part->id << " startTime " << part->startTime);
		ZENIC_INFO("Part " << i << " syncPoints " << parts[i]->syncPoints.count());
	}

#endif

	stream.close();
	File::setMode(oldMode);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}


