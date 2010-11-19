#ifndef zenic_net_win32_Socket_h
#define zenic_net_win32_Socket_h

#include "../Address.h"
#if defined(_MSC_VER)
#pragma warning(disable: 4496)
#endif
#include <winsock2.h>
#if defined(_MSC_VER)
#pragma warning(default: 4496)
#endif

namespace zenic
{

namespace net
{

class Socket
{
public:

	Socket();
	virtual ~Socket();

	bool setBlocking( bool blocking );

	bool connect();
	bool listen( uint backlog = 5 );
	bool close();
	Socket* accept();

	bool send( const void* buffer, uint length );
	int receive( void* buffer, uint length );

	SOCKET internalSocket() const;
	void setInternalSocket( SOCKET internalSocket );

	const Address& address() const;
	void setAddress( const Address& address );

private:

	SOCKET m_socket;
	Address m_address;
};

#include "Socket.inl"

}

}

#endif
