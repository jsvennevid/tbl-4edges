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

#ifndef zenic_FileStream_h
#define zenic_FileStream_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Stream.h"
#include "File.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FileStream : public Stream
{
public:

	enum Mode
	{
		Read,
		Write
	};

	FileStream();
	virtual ~FileStream();

	bool open(const char* filename, Mode mode);
	void close();

	virtual int read(void* buffer, int size);
	virtual int write(const void* buffer, int size);
	virtual int seek(SeekMode mode, int offset);
	virtual bool flush();
	virtual int tell() const;

	bool isGood() const;

private:

	enum
	{
		BufferSize = 4096
	};

	File m_file;
	Mode m_mode;

	u8 m_buffer[BufferSize];
	int m_offset;
	int m_size;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FileStream.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
