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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

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
	DWORD desiredAccess = (Read == mode) ? GENERIC_READ : GENERIC_WRITE;
	DWORD shareMode = (Read == mode) ? FILE_SHARE_READ : 0;
	DWORD creationDisposition = (Read == mode) ? OPEN_EXISTING : (Append == mode) ? OPEN_ALWAYS : CREATE_ALWAYS;

	m_file = CreateFile(filename, desiredAccess, shareMode, NULL, creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_file ==  INVALID_HANDLE_VALUE)
		return false;

	setGood(true);
	setEof(false);

	if (Append == mode)
		return (seek(End, 0) != InvalidOffset);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void File::close()
{
	if (m_file)
		CloseHandle(m_file);

	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::Size File::read(void* buffer, Size size)
{
	if (!isGood())
		return InvalidSize;

	DWORD readData;
	if (!ReadFile(m_file, buffer, size, &readData, NULL))
	{
		setGood(false);
		setEof(true);

		return InvalidSize;
	}

	if (!readData)
		setEof(true);

	return readData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::Size File::write(const void* buffer, Size size)
{
	if (!isGood())
		return InvalidSize;

	DWORD writtenData;
	if (!WriteFile(m_file, buffer, size, &writtenData, NULL ))
	{
		setGood(false);
		setEof(true);

		return InvalidSize;
	}

	return writtenData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::Offset File::seek(Location location, Offset offset)
{
	if (!isGood())
		return InvalidOffset;

	DWORD seekMethod;
	switch (location)
	{
		default: 
		case Start: seekMethod = FILE_BEGIN; break;
		case Current: seekMethod = FILE_CURRENT; break;
		case End: seekMethod = FILE_END; break;
	}

	LARGE_INTEGER largeOffset;
	largeOffset.QuadPart = offset;
	largeOffset.LowPart = SetFilePointer(m_file, largeOffset.LowPart, &largeOffset.HighPart, seekMethod);

	if ((INVALID_SET_FILE_POINTER == largeOffset.LowPart) && (GetLastError() != NO_ERROR))
		return InvalidOffset;

	return Offset(largeOffset.QuadPart);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::Offset File::tell() const
{
	if (!isGood())
		return InvalidOffset;

	LARGE_INTEGER largeOffset;
	largeOffset.QuadPart = 0;
	largeOffset.LowPart = SetFilePointer(m_file, largeOffset.LowPart, &largeOffset.HighPart, FILE_CURRENT);

	if ((INVALID_SET_FILE_POINTER == largeOffset.LowPart) && (GetLastError() != NO_ERROR))
		return InvalidOffset;

	return Offset(largeOffset.QuadPart);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
