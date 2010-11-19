inline BufferStream::BufferStream() : m_offset(0), m_internalOffset(0)
{
}

inline BufferStream::BufferStream(DataPtr<u8> buffer) : m_offset(0), m_internalOffset(0), m_buffer(buffer)
{
}

inline void BufferStream::setBuffer(DataPtr<u8> buffer)
{
	m_internalOffset = 0;
	m_offset = 0;
	m_buffer = buffer;
}

inline DataPtr<u8> BufferStream::buffer()
{
	flush();

	u32 count = m_buffer.count() ? m_buffer.count()-1 : 0;
	return DataPtr<u8>(m_buffer.objects(),count,0);
}

inline const c8* BufferStream::string()
{
	flush();
	return reinterpret_cast<const c8*>(m_buffer.objects());
}
