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

#include "FileSystem.h"
#include <Shared/Base/Io/Log.h>
#include <fileXio_rpc.h>
#include <loadfile.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

FileSystem* FileSystem::m_instance = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FileSystem::FileSystem() : 
	m_driveName("pfs0:/"),
	m_mode(Cdvd),
	m_hasLodedIrx(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FileSystem::setIoMode(Mode mode)
{
	if (mode == Hdd)
	{
		if (!m_hasLodedIrx)
		{
			static char hddarg[] = "-o" "\0" "4" "\0" "-n" "\0" "20";
			static char pfsarg[] = "-m" "\0" "4" "\0" "-o" "\0" "10" "\0" "-n" "\0" "40";

			SifLoadModule("host:data/Irx/fileXio.irx", 0, 0);
			SifLoadModule("host:data/Irx/ps2atad.irx", 0, 0);
			SifLoadModule("host:data/Irx/ps2hdd.irx", sizeof(hddarg), hddarg);
			SifLoadModule("host:data/Irx/ps2fs.irx", sizeof(pfsarg), pfsarg);
			fileXioMount("pfs0:", "hdd:+WOrk", O_RDONLY);

			m_hasLodedIrx = true;
		}
	}

	switch (mode)
	{
		case Host : m_driveName = "host:"; break;
		case Hdd : m_driveName = "pfs0:/"; break;
		case Cdvd : m_driveName = "cdrom0:\\"; break;
		case DevkitHost : m_driveName = "host0:"; break;
	}

	m_mode = mode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileSystem::open(FileStream& stream, const char* filename, FileStream::Mode mode)
{
	char temp[256];

	// TEMP, implement proper operator into String class instead

	sprintf(temp, "%s%s", m_driveName.contents(), filename);
	ZENIC_INFO("Trying to open file " << temp);

	if (m_mode == Hdd)
		File::setMode(File::IomanX);

	return stream.open(temp, mode);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FileSystem::fullPath(char* fullFilename, const char* filename)
{
	sprintf(fullFilename, "%s%s", m_driveName.contents(), filename);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FileSystem& FileSystem::instance()
{
	if (!m_instance)
		m_instance = zenic_new FileSystem;

	ZENIC_ASSERT(m_instance);
	return *m_instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}
