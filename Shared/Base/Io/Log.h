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

#ifndef zenic_Log_h
#define zenic_Log_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../Io/Stream.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Log : public Stream
{
public:

	Log();
	virtual ~Log();

private:

	virtual int read(void* buffer, int size);
	virtual int write(const void* buffer, int size);
	virtual int seek(SeekMode mode, int offset);
	virtual bool flush();
	virtual int tell() const;

	enum { BufferSize = 511 };

	int m_offset;
	c8 m_buffer[ BufferSize+1 ];
};

extern Log logOut;

}

#define ZENIC_ERROR(a) zenic::logOut << __FILE__ <<  "(" << __LINE__ << ")" << " [ERROR] " << a << zenic::endl
#define ZENIC_WARNING(a) zenic::logOut << __FILE__ <<  "(" << __LINE__ << ")" << " [WARNING] " << a << zenic::endl
#define ZENIC_INFO(a) zenic::logOut << __FILE__ <<  "(" << __LINE__ << ")" << " [INFO] " << a << zenic::endl
#define ZENIC_VERIFY(exp,a) if (!(exp)) ZENIC_WARNING(a)

#endif //zenic_log_h
