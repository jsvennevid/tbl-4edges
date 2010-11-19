#include "SocketManager.h"

#include "../Socket.h"
#include "../Address.h"

#include <Shared/Base/Storage/String.h>

#if defined(_MSC_VER)
#pragma warning(disable: 4127 4706 4700 4996)
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#if defined(_MSC_VER)
#pragma warning(default: 4127 4706 4700 4996)
#endif

namespace zenic
{

namespace net
{

SocketManager::SocketManager()
{
}

SocketManager::~SocketManager()
{
}

Socket* SocketManager::createTcpSocket( const zenic::net::Address& address )
{
	SOCKET socketHandle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( INVALID_SOCKET == socketHandle )
		return 0;

	sockaddr* saddr = reinterpret_cast<sockaddr*>(address.data().objects());
	int size = address.data().count();
	int err = ::bind(socketHandle, saddr, size);
	if( SOCKET_ERROR == err )
	{
		::closesocket(socketHandle);
		return 0;
	}

	Socket* socket = zenic_new Socket;
	ZENIC_ASSERT( socket );

	socket->setInternalSocket( socketHandle );
	socket->setAddress( address );

	return socket;
}

Socket* SocketManager::createUdpSocket( const Address& address )
{
	SOCKET socketHandle = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( INVALID_SOCKET == socketHandle )
		return 0;

	sockaddr* saddr = reinterpret_cast<sockaddr*>(address.data().objects());
	int size = address.data().count();
	int err = ::bind(socketHandle, saddr, size);
	if( SOCKET_ERROR == err )
	{
		::closesocket(socketHandle);
		return 0;
	}

	Socket* socket = zenic_new Socket;
	ZENIC_ASSERT( socket );

	socket->setInternalSocket( socketHandle );
	socket->setAddress( address );

	return socket;
}

void SocketManager::destroySocket( Socket* socket )
{
	ZENIC_ASSERT( socket );
	zenic_delete socket;
}

bool SocketManager::stringToAddress( const String& string, Address& address ) const
{
	SOCKADDR saddr;
	INT size = sizeof(saddr);

	INT error = WSAStringToAddress(const_cast<LPSTR>(string.contents()), AF_INET, 0, &saddr, &size );
	if( SOCKET_ERROR == error )
		return false;

	address.setData( &saddr, size );

	return true;
}

bool SocketManager::nameToAddress( const String& name, Address& address ) const
{
	String hostname;
	u16 port;

	addrinfo hints;
	struct addrinfo* addressInfo;
	struct addrinfo* ipv4Info;
	DWORD error;

	if( !splitName(name,hostname,port) )
		return false;

	if( hostname.empty() )
	{
		sockaddr_in saddr;
		memset(&saddr,0,sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		saddr.sin_port = htons(port);
		address.setData(&saddr,sizeof(saddr));
		return true;
	}	

	memset(&hints,0,sizeof(hints));
	hints.ai_flags = AI_NUMERICHOST|AI_PASSIVE;
	hints.ai_family = PF_UNSPEC;

	error = getaddrinfo(hostname.contents(),0,&hints,&addressInfo);
	if( EAI_NONAME != error )
	{
		sockaddr_in saddr;
		ZENIC_ASSERT( addressInfo->ai_addrlen == sizeof(saddr) );

		::memcpy( &saddr, addressInfo->ai_addr, sizeof(saddr) );
		saddr.sin_port = htons(port);
		freeaddrinfo(addressInfo);

		address.setData(&saddr,sizeof(saddr));
		return true;
	}

	hints.ai_flags = AI_PASSIVE;
	error = getaddrinfo(hostname.contents(),0,&hints,&addressInfo);
	if( error )
		return false;

	ipv4Info = addressInfo;
	while( ipv4Info->ai_family != AF_INET )
	{
		ipv4Info = ipv4Info->ai_next;
		if( !ipv4Info )
		{
			freeaddrinfo(addressInfo);
			return false;
		}
	}

	sockaddr_in saddr;
	ZENIC_ASSERT( ipv4Info->ai_addrlen == sizeof(saddr) );

	::memcpy( &saddr, ipv4Info->ai_addr, sizeof(saddr) );
	saddr.sin_port = htons(port);
	freeaddrinfo(addressInfo);

	address.setData(&saddr,sizeof(saddr));
	return true;
}

bool SocketManager::addressToName( const Address& address, String& name ) const
{
	sockaddr* saddr = reinterpret_cast<sockaddr*>(address.data().objects());
	int size = address.data().count();

	CHAR buffer[1024];
	DWORD maxLength = sizeof(buffer);

	INT error = WSAAddressToString(saddr, size, 0, buffer, &maxLength);
	if( SOCKET_ERROR == error )
		return false;

	name = buffer;
	return true;
}

bool SocketManager::splitName( const String& name, String& hostname, u16& port )
{
	int offset = name.find(":");
	if (offset < 0)
		return false;

	hostname = name.subString(0, offset);
	port = (u16)::atoi(&(name.contents()[offset+1]));

	return true;
}

}

}
