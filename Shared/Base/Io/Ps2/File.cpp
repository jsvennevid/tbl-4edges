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

#include "File.h"
#include <Shared/Base/Io/Log.h>
#include <fileXio_rpc.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

File::IoMode File::s_ioMode = File::Ioman;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::File()
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::~File()
{
	close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool File::open(const char* filename, Mode mode)
{
	ZENIC_INFO(__LINE__);

	if (s_ioMode == Ioman)
	{
		ZENIC_INFO(__LINE__);

		const char* fileMode = 0;
	
		switch (mode)
		{
			case Read : fileMode = "rb"; break;
			case Write : fileMode = "wb"; break;
			case Append : fileMode = "ab"; break;
		}
	
		ZENIC_INFO(__LINE__);
		m_file = fopen(filename, fileMode);
		ZENIC_INFO(__LINE__);
	
		if (!m_file)
			return false;
	
	}
	else
	{
		int fileMode = 0;

		switch (mode)
		{
			case Read : fileMode = O_RDONLY; break;
			case Write : fileMode = O_WRONLY; break;
			case Append : fileMode = O_APPEND; break;
		}

		ZENIC_INFO(__LINE__);
		m_handle = fileXioOpen(filename, fileMode, 0);
		ZENIC_INFO(__LINE__);

		if (m_handle < 0)
			return false;
	}

	ZENIC_INFO(__LINE__);

	setGood(true);
	setEof(false);

	if (Append == mode)
		return (seek(End, 0) != InvalidOffset);

	ZENIC_INFO(__LINE__);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void File::close()
{
	if (s_ioMode == Ioman)
	{
		if (m_file)
			fclose(m_file);
	}
	else
	{
		ZENIC_INFO("___ About to close " << m_handle);
		
		if (m_handle > 0)
		{
			fileXioClose(m_handle);
			ZENIC_INFO("XXX Closed " << m_handle);
		}
	}

	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::Size File::read(void* buffer, Size size)
{
	if (!isGood())
		return InvalidSize;

	File::Size readSize = 0;

	if (s_ioMode == Ioman)
		readSize = fread(buffer, size, 1, m_file);
	else
		readSize = fileXioRead(m_handle, reinterpret_cast<u8*>(buffer), size);

	if (readSize < 0)
	{
		setGood(false);
		setEof(true);

		return InvalidSize;
	}

	if (!readSize)
		setEof(true);
	
	// this seems to be a bug in ps2sdk as this should be readSize
	return size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::Size File::write(const void* buffer, Size size)
{
	if (!isGood())
		return InvalidSize;

	File::Size writeSize = 0;

	if (s_ioMode == Ioman)
		writeSize = fwrite(buffer, size, 1, m_file);
	else
		writeSize = fileXioWrite(m_handle, (u8*)buffer, size);

	if (writeSize < 0)
	{
		setGood(false);
		setEof(true);

		return InvalidSize;
	}

	// this seems to be a bug in ps2sdk as this should be writeSize
	return size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::Offset File::seek(Location location, Offset offset)
{
	if (!isGood())
		return InvalidOffset;

	File::Offset seekOffset = 0;

	uint seekMethod = 0;

	switch (location)
	{
		case Start: seekMethod = SEEK_SET; break;
		case Current: seekMethod = SEEK_CUR; break;
		case End: seekMethod = SEEK_END; break;
	}


	if (s_ioMode == Ioman)
		seekOffset = fseek(m_file, seekMethod, offset);		
	else
		seekOffset = fileXioLseek(m_handle, offset, seekMethod);

	if (seekOffset < 0)
		return InvalidOffset;

	return seekOffset;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::Offset File::tell() const
{
	if (!isGood())
		return InvalidOffset;

	if (s_ioMode == Ioman)
		return ftell(m_file);
	else
		return fileXioLseek(m_handle, 0, SEEK_CUR);	// might not be correct

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
