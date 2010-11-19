#include "HttpServer.h"

#include "HttpRequest.h"
#include "../NetworkManager.h"
#include "../SocketManager.h"

#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Storage/String.h>

namespace zenic
{

namespace net
{

HttpServer::HttpServer() : m_socket(0)
{
}

HttpServer::~HttpServer()
{
}

bool HttpServer::create()
{
	SocketManager* socketManager = NetworkManager::socketManager();
	if( !socketManager )
		return false;

	Address address;
	if( !socketManager->stringToAddress("0.0.0.0:80",address) )
		return false;

	m_socket = socketManager->createTcpSocket(address);
	if( !m_socket )
	{
		ZENIC_ERROR( "Could not create socket" );
		return false;
	}

	if( !m_socket->setBlocking(false) )
	{
		ZENIC_ERROR( "Could not set non-blocking mode" );
		return false;
	}

	if( !m_socket->listen() )
	{
		ZENIC_ERROR( "Could not listen to socket" );
		return false;
	}

	return true;
}

void HttpServer::destroy()
{
	SocketManager* socketManager = NetworkManager::socketManager();
	if( !socketManager )
		return;

	if( m_socket )
	{
		socketManager->destroySocket( m_socket );
		m_socket = 0;
	}
}

bool HttpServer::update()
{
	m_select.reset();

	m_select.addReader( m_socket );

	for( uint i = 0, n = m_connections.count(); i < n; ++i )
	{
		Connection* connection = m_connections[i];
		if( !connection )
			continue;

		ZENIC_ASSERT( connection->m_socket );
		m_select.addReader( connection->m_socket );
	}

	if( m_select.query() )
	{
		if( m_select.isReadable( m_socket ) )
		{
			Socket* newSocket = m_socket->accept();
			if( newSocket )
			{
				ZENIC_INFO( "New connection to server" );

				Connection* connection = new Connection;
				ZENIC_ASSERT( connection );

				connection->m_socket = newSocket;
				connection->m_request = 0;

				for( uint i = 0; i < m_connections.count(); ++i )
				{
					if( !m_connections[i] )
					{
						m_connections[i] = connection;
						connection = 0;
						break;
					}
				}

				if( connection )
					m_connections.pushBack( connection );
			}
		}
	}


	for( uint i = 0; i < m_connections.count(); ++i )
	{
		Connection* connection = m_connections[i];
		if( !connection )
			continue;

		Socket* socket = connection->m_socket;
		ZENIC_ASSERT( socket );

		if( !m_select.isReadable( socket ) )
			continue;

		u8 buffer[2048];
		int size = socket->receive(buffer,sizeof(buffer));
		if( size >= 0 )
		{
			if( !connection->m_request )
				connection->m_request = new HttpRequest;

			connection->m_buffer.write( buffer, size );

			HttpRequest* request = connection->m_request;
			ZENIC_ASSERT( request );

			if( !request->parse( connection->m_buffer ) )
				ZENIC_INFO( "Parse error" );

			if( request->isComplete() )
			{
				ZENIC_INFO( "Request OK" );

				socket->close();
				delete connection->m_request;
				delete connection;
				m_connections[i] = 0;
			}
		}
		else if( size < 0 )
		{
			socket->close();
			delete connection->m_request;
			delete connection;
			m_connections[i] = 0;
		}
	}
	m_connections.compress();

	return true;
}

}

}
