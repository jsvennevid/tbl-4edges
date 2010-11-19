#include <Shared/Base/Log/Log.h>
#include <Shared/Net/NetworkManager.h>
#include <Shared/Net/Debug/HttpServer.h>
#include <Shared/Base/Io/FifoStream.h>

int main(int argc, char* argv[])
{
	if( !zenic::net::NetworkManager::create() )
	{
		ZENIC_ERROR( "Could not start network" );
		return 1;
	}

	zenic::net::HttpServer server;
	if( !server.create() )
	{
		ZENIC_ERROR( "Failed creating httpserver" );
		return 1;
	}

	while( 1 )
	{
		server.update();
		SleepEx(0,false);
	}

	server.destroy();

	if( !zenic::net::NetworkManager::destroy() )
	{
		ZENIC_ERROR( "Error occured while shutting down network" );
		return 1;
	}

	return 0;
}
