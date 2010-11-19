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

#ifndef zenic_psp_File_h
#define zenic_psp_File_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../Types.h"

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class File
{
public:

	enum Mode
	{
		Read,
		Write,
		Append
	};

	enum Location
	{
		Start,
		Current,
		End
	};

	typedef s64 Offset;
	typedef s32 Size;

	enum { InvalidSize = -1, InvalidOffset = -1 };

	File();
	~File();

	bool open(const char* filename, Mode mode);
	void close();

	bool isGood() const;
	bool isBad() const;
	bool isEof() const;

	Size read(void* buffer, Size size);
	Size write(const void* buffer, Size size);

	Offset seek(Location location, Offset offset);
	Offset tell() const;

private:

	void reset();

	void setGood(bool good);
	void setEof(bool eof);

	bool m_good:1;
	bool m_eof:1;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "File.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif //zenic_ps2_File_h

