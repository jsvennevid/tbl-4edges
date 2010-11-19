#ifndef zenic_net_HttpRequest_h
#define zenic_net_HttpRequest_h

#include <Shared/Base/Io/BufferStream.h>
#include <Shared/Base/Storage/Array.h>
#include <Shared/Base/Storage/String.h>

namespace zenic
{
	class Stream;
}

namespace zenic
{

namespace net
{

class HttpRequest
{
public:

	enum Type
	{
		Invalid,
		Get,
		Put,
		Head,
		Post
	};

	class Range
	{
	public:
		int m_start;
		int m_end;
	};

	HttpRequest();
	~HttpRequest();

	bool parse( Stream& input );
	bool isComplete() const;
	bool isError() const;

	Type type() const; 

private:

	enum ParseMode
	{
		Request,
		Header,
		Finished,
		Error
	};

	bool parseRequest( const String& string );
	bool parseHeader( const String& string );

	String m_input;
	ParseMode m_mode;

	BufferStream m_buffer;

	Type m_type;
	String m_uri;

	// headers

	String m_accept;
	String m_connection;
	int m_contentLength;
	String m_contentType;
	String m_expect;
	String m_date;
	String m_host;
	String m_ifModifiedSince;
	Array<Range> m_ranges;
	String m_referer;
	String m_transferEncoding;
	String m_userAgent;
	Array<String> m_customHeaders;
};

}

}

#endif
