#include "Select.h"

#include "../Socket.h"

#include <math.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4127 4800 )
#endif

namespace zenic
{

namespace net
{

Select::Select()
{
	reset();
}

Select::~Select()
{
}

void Select::reset()
{
	FD_ZERO(&m_readers);
	FD_ZERO(&m_writers);
}

void Select::addReader( Socket* socket )
{
	SOCKET s = socket->internalSocket();
	ZENIC_ASSERT( s != INVALID_SOCKET );

	FD_SET(s,&m_readers);
}

void Select::addWriter( Socket* socket )
{
	SOCKET s = socket->internalSocket();
	ZENIC_ASSERT( s != INVALID_SOCKET );

	FD_SET(s,&m_writers);
}

bool Select::isReadable( Socket* socket ) const
{
	SOCKET s = socket->internalSocket();
	ZENIC_ASSERT( s != INVALID_SOCKET );

	return static_cast<bool>(FD_ISSET(s,&m_readers));
}

bool Select::isWritable( Socket* socket ) const
{
	SOCKET s = socket->internalSocket();
	ZENIC_ASSERT( s != INVALID_SOCKET );

	return static_cast<bool>(FD_ISSET(s,&m_writers));
}

bool Select::query( bool block, float timeout )
{
	fd_set* readers = 0;
	fd_set* writers = 0;

	if( m_readers.fd_count > 0 )
		readers = &m_readers;

	if( m_writers.fd_count > 0 )
		writers = &m_writers;

	if( !readers || !writers )
		return true;

	int error = 0;
	if( block )
		error = ::select( 0, readers, writers, 0, 0 );
	else
	{
		timeval tval;
		if( timeout > 0 )
		{
			float seconds;
			float fraction = modff(timeout,&seconds);

			tval.tv_sec = static_cast<uint>(seconds);
			tval.tv_usec = static_cast<uint>(fraction * 1000000);
		}
		else
		{
			tval.tv_sec = 0;
			tval.tv_usec = 0;
		}

		error = ::select( 0, readers, writers, 0, &tval );
	}

	if( error < 0 )
		return false;

	if( !error )
	{
		reset();
	}

	return true;
}


}

}
