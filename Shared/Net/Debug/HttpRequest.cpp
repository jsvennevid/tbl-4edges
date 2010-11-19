#include "HttpRequest.h"

#include <Shared/Base/Io/Log.h>

#include <string.h>
#include <stdlib.h>

namespace zenic
{

namespace net
{

HttpRequest::HttpRequest() : m_mode(Request), m_type(Invalid), m_contentLength(-1)
{
}

HttpRequest::~HttpRequest()
{
}

bool HttpRequest::parse( Stream& input )
{
	for(;;)
	{
		c8 c;
		int err = input.read(&c,sizeof(c8));
		if( err < 0 )
			return false;

		if( err > 0 )
		{
			if( c == '\n' )
			{
				m_buffer << ends;

				switch( m_mode )
				{
					case Request:
					{
						if( !parseRequest( m_buffer.string() ) )
							return false;

						m_mode = Header;
					}
					break;

					case Header:
					{
						if( ::strlen( m_buffer.string() ) )
						{
							if( !parseHeader( m_buffer.string() ) )
								return false;
						}
						else
						{
							m_mode = Finished;
							return true;
						}
					}
					break;

					default:
						return false;
				}

				// reset buffer

				m_buffer.seek(Stream::Start,0);
			}
			else if( c != '\r' )
				m_buffer.write(&c,sizeof(c8));
		}
	}
}

bool HttpRequest::parseRequest( const String& string )
{
	uint len = string.length();

	String type;
	uint i;
	for( i = 0; (i<len)&&(' ' != string[i]); ++i );

	if( i == len )
		return false;

	if( !String::icompare("GET",string,i) )
		m_type = Get;
	else if( !String::icompare("HEAD",string,i) )
		m_type = Head;
	else if( !String::icompare("POST",string,i) )
		m_type = Post;
	else
		return false;

	uint j;
	for( j = i+1; (j<len)&&(' ' != string[j]); ++j );

	if( j == len )
		return false;

	m_uri = string.subString(i+1,j);

	uint k = j+1;

	if( String::icompare("HTTP/",string,5) )
		return false;
	k+=5;

	// TODO: extract protocol version

	return true;
}

bool HttpRequest::parseHeader( const String& string )
{
	uint len = string.length();

	uint i;
	for( i = 0; i < len; ++i )
	{
		if(':' == string[i])
			break;
	}

	if( i == len )
		return false;

	uint j;
	for( j = i+1; i < len; ++j )
	{
		if( ' ' != len )
			break;
	}

	if ( !String::icompare("Accept", string, i) )
		m_accept = string+j;
	else if ( !String::icompare("Connection", string, i) )
		m_connection = string+j;
	else if ( !String::icompare("Content-Length", string, i) )
		m_contentLength = ::atoi(string+j);
	else if ( !String::icompare("Content-Type", string, i) )
		m_expect = string+j;
	else if ( !String::icompare("Date", string, i) )
		m_date = string+j;
	else if ( !String::icompare("Host", string, i) )
		m_host = string+j;
	else if ( !String::icompare("If-Modified-Since", string, i) )
		m_ifModifiedSince = string+j;
	else if ( !String::icompare("Referer", string, i) )
		m_referer = string+j;
	else if ( !String::icompare("Transfer-Encoding", string, i) )
		m_transferEncoding = string+j;
	else if( !String::icompare("User-Agent", string, i) )
		m_userAgent = string+j;

	return true;
}

bool HttpRequest::isComplete() const
{
	return m_mode == Finished;
}

bool HttpRequest::isError() const
{
	return m_mode == Error;
}

}

}
