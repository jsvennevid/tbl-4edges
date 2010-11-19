#ifndef zenic_net_win32_SocketManager_h
#define zenic_net_win32_SocketManager_h

#include <Shared/Base/Types.h>

namespace zenic
{
	class String;

namespace net
{
	class Socket;
	class Address;
}

}

namespace zenic
{

namespace net
{

class SocketManager
{
public:

	SocketManager();
	~SocketManager();

	Socket* createTcpSocket( const Address& address );
	Socket* createUdpSocket( const Address& address );
	void destroySocket( Socket* socket );

	bool stringToAddress( const String& string, Address& address ) const;
	bool nameToAddress( const String& name, Address& address ) const;

	bool addressToName( const Address& address, String& name ) const;

private:

	static bool splitName( const String& name, String& hostname, u16& port );
};

}

}

#endif
