#include "NetworkManager.h"

#include "../SocketManager.h"

#include <Shared/Base/Memory/New.h>
#include <Shared/Base/Debug/Assert.h>

#include <winsock2.h>

namespace zenic
{

namespace net
{

SocketManager* NetworkManager::m_socketManager = 0;

bool NetworkManager::create()
{
	WORD versionRequested;
	WSADATA wsaData;
	int error;

	versionRequested = MAKEWORD(2,2);

	error = WSAStartup(versionRequested,&wsaData);
	if( error )
		return false;

	if( (LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion != 2)) )
	{
		WSACleanup();
		return false;
	}

	m_socketManager = zenic_new SocketManager;
	ZENIC_ASSERT( m_socketManager );

	return true;
}

bool NetworkManager::destroy()
{
	zenic_delete m_socketManager;
	m_socketManager = 0;

	int error = WSACleanup();
	if( error == SOCKET_ERROR )
		return false;

	return true;
}

}

}
