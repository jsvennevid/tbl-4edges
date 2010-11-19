#ifndef zenic_net_HttpHandler_h
#define zenic_net_HtppHandler_h

namespace zenic
{

namespace net
{
	class HttpRequest;
}

}

namespace zenic
{

namespace net
{

class HttpHandler
{
public:

	virtual ~HttpHandler();

	virtual bool process( HttpRequest* request ) = 0;

private:
};

}

}

#endif
