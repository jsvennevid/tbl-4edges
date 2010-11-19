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
#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

FileSystem* FileSystem::m_instance = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FileSystem::FileSystem() : 
	m_driveName("")
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileSystem::open(FileStream& stream, const char* filename, FileStream::Mode mode)
{
	char temp[256];

	// TEMP, implement proper operator into String class instead

	sprintf(temp, "%s%s", m_driveName.contents(), filename);
	ZENIC_INFO("Trying to open file " << temp);

	return stream.open(temp, mode);
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
