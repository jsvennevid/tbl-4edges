inline Address::Address()
{
}

inline Address::Address( const Address& address )
{
	*this = address;
}

inline Address::~Address()
{
	clear();
}

inline Address& Address::operator = ( const Address& address )
{
	m_data.reserve( address.data().count() );
	m_data.copy( address.data() );
	return *this;
}

inline bool Address::operator == ( const Address& address ) const
{
	if( m_data.count() != address.data().count() )
		return false;

	if( ::memcmp(m_data.objects(),address.data().objects(),m_data.count()) )
		return false;

	return true;
}

inline bool Address::operator != ( const Address& address ) const
{
	return !(*this == address);
}

inline void Address::setData( void* data, uint size )
{
	m_data.reserve( size );
	m_data.copy(DataPtr<u8>(reinterpret_cast<u8*>(data),size,0));
}

inline DataPtr<u8> Address::data() const
{
	return m_data;
}

inline void Address::clear()
{
	m_data.free();
}

inline bool Address::isSet() const
{
	return m_data != 0;
}
