#ifndef zenic_net_Address_h
#define zenic_net_Address_h

#include <Shared/Base/Types.h>
#include <Shared/Base/Memory/DataPtr.h>

#include <string.h>

namespace zenic
{

namespace net
{

class Address
{
public:

	Address();
	Address( const Address& address );
	~Address();

	Address& operator = ( const Address& address );
	bool operator == ( const Address& address ) const;
	bool operator != ( const Address& address ) const;

	void setData( void* data, uint size );
	DataPtr<u8> data() const;

	void clear();
	bool isSet() const;

private:

	DataPtr<u8> m_data;
};

#include "Address.inl"

}

}

#endif
