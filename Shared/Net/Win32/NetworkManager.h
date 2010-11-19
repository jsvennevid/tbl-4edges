#ifndef zenic_net_win32_NetworkManager_h
#define zenic_net_win32_NetworkManager_h

namespace zenic
{

namespace net
{
	class SocketManager;
}

}

namespace zenic
{

namespace net
{

class NetworkManager
{
public:

	static bool create();
	static bool destroy();

	static SocketManager* socketManager();

private:

	NetworkManager();
	~NetworkManager();

	static SocketManager* m_socketManager;
};

#include "NetworkManager.inl"

}

}

#endif
