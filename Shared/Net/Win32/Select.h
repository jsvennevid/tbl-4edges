#ifndef zenic_net_win32_Select_h
#define zenic_net_win32_Select_h

#include <Shared/Base/Storage/Array.h>

#include <winsock2.h>

namespace zenic
{

namespace net
{
	class Socket;
}

}

namespace zenic
{

namespace net
{

class Select
{
public:

	Select();
	~Select();

	void reset();
	bool query( bool block = false, float timeout = -1.0f );

	void addReader( Socket* socket );
	void addWriter( Socket* socket );

	bool isReadable( Socket* socket ) const;
	bool isWritable( Socket* socket ) const;

private:

	fd_set m_readers;
	fd_set m_writers;
};

}

}

#endif
