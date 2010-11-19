inline Socket::Socket() : m_socket(INVALID_SOCKET)
{
}

inline Socket::~Socket()
{
	close();
}

inline SOCKET Socket::internalSocket() const
{
	return m_socket;
}

inline void Socket::setInternalSocket( SOCKET internalSocket )
{
	m_socket = internalSocket;
}

inline const Address& Socket::address() const
{
	return m_address;
}

inline void Socket::setAddress( const Address& address )
{
	m_address = address;
}
