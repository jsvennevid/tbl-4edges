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
