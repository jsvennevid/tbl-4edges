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
#ifndef zenic_BufferStream_h
#define zenic_BufferStream_h

#include "Stream.h"

#include "../Memory/DataPtr.h"

namespace zenic
{

class BufferStream : public Stream
{
public:

	BufferStream();
	BufferStream(DataPtr<u8> buffer);
	virtual ~BufferStream();

	void setBuffer(DataPtr<u8> buffer);
	DataPtr<u8> buffer();
	const c8* string();

	virtual int read(void* buffer, int size);
	virtual int write(const void* buffer, int size);
	virtual int seek(SeekMode mode, int offset);
	virtual bool flush();
	virtual int tell() const;

private:

	enum { BufferSize = 256 };

	int m_offset;	
	int m_internalOffset;

	DataPtr<u8> m_buffer;
	u8 m_internalBuffer[BufferSize];
};

#include "BufferStream.inl"

}

#endif
