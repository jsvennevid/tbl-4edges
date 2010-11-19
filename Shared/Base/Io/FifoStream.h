#ifndef zenic_FifoStream_h
#define zenic_FifoStream_h

#include "Stream.h"

namespace zenic
{

class FifoStream : public Stream
{
public:

	FifoStream();
	virtual ~FifoStream();

	virtual int read(void* buffer, int size);
	virtual int write(const void* buffer, int size);

private:

	virtual int seek(SeekMode mode, int offset);
	virtual bool flush();
	virtual int tell() const;

	DataPtr<u8> m_buffer;
	int m_reading;
	int m_writing;
};

}

#endif
