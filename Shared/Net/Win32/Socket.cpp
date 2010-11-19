#include "Socket.h"

namespace zenic
{

namespace net
{

bool Socket::setBlocking( bool blocking )
{
	unsigned long block = blocking ? 0 : 1;
	int error = ::ioctlsocket( m_socket, FIONBIO, &block );
	if( SOCKET_ERROR == error )
		return false;
	return true;
}

bool Socket::connect()
{
	if( ::connect(m_socket,reinterpret_cast<const sockaddr*>(m_address.data().objects()),m_address.data().count()) == SOCKET_ERROR )
	{
		if( WSAEWOULDBLOCK != WSAGetLastError() )
			return false;
	}

	return true;
}

bool Socket::listen( uint backlog )
{
	if( ::listen(m_socket,backlog) == SOCKET_ERROR )
		return false;

	return true;
}

bool Socket::close()
{
	int error = ::closesocket(m_socket);
	m_socket = INVALID_SOCKET;

	if( SOCKET_ERROR == error )
		return false;
	return true;
}

Socket* Socket::accept()
{
	SOCKET s = ::accept( m_socket, 0, 0 );
	if( s == INVALID_SOCKET )
		return 0;

	Socket* socket = new Socket;
	socket->setInternalSocket( s );
	// TODO: extract addressinfo

	return socket;
}

bool Socket::send( const void* buffer, uint length )
{
	const sockaddr* address = reinterpret_cast<sockaddr*>(m_address.data().objects());

	ZENIC_ASSERT( INVALID_SOCKET != m_socket );

	int error = ::sendto( m_socket, reinterpret_cast<const char*>(buffer), length, 0, address, sizeof(sockaddr_in) );
	if( error >= 0 )
		return true;

	error = GetLastError();
	//if( WSAEWOULDBLOCK != error )

	return false;
}

int Socket::receive( void* buffer, uint length )
{
	sockaddr_in from;
	int size = sizeof(sockaddr_in);

	int readSize = ::recvfrom( m_socket, reinterpret_cast<char*>(buffer), length, 0, reinterpret_cast<sockaddr*>(&from), &size );
	if( readSize >= 0 )
	{
		m_address.setData(&from,size);
		return readSize;
	}

	int error = WSAGetLastError();
	if( WSAEWOULDBLOCK == error )
		return 0;

	m_address.clear();
	return -1;
}

}

}
