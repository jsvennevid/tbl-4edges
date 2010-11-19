#ifndef zenic_net_HttpServer_h
#define zenic_net_HttpServer_h

#include "../Socket.h"
#include "../Select.h"

#include <Shared/Base/Io/FifoStream.h>
#include <Shared/Base/Storage/Array.h>

namespace zenic
{

namespace net
{
	class HttpRequest;
	class HttpHandler;
}

}

namespace zenic
{

namespace net
{

class HttpServer
{
public:

	HttpServer();
	~HttpServer();

	bool create();
	void destroy();

	bool update();

private:

	class Connection
	{
	public:
		Socket* m_socket;
		FifoStream m_buffer;

		HttpRequest* m_request;
	};

	Socket* m_socket;
	Select m_select;

	Array<Connection*> m_connections;
};

}

}

#endif
