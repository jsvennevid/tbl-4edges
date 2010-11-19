inline ModelData::ModelData() : m_type(0), m_numVertices(0), m_numPrimitives(0), m_indexFormat(0)
{
}

inline void ModelData::setType(u32 type)
{
	m_type = type;
}

inline u32 ModelData::type() const
{
	return m_type;
}

inline void ModelData::setNumVertices(u32 numVertices)
{
	m_numVertices = numVertices;
}

inline u32 ModelData::numVertices() const
{
	return m_numVertices;
}

inline void ModelData::setNumPrimitives(u32 numPrimitives)
{
	m_numPrimitives = numPrimitives;
}

inline u32 ModelData::numPrimitives() const
{
	return m_numPrimitives;
}

inline void ModelData::setNumStreams(u32 streams)
{
	m_streams.resize(streams);
}

inline u32 ModelData::numStreams() const
{
	return m_streams.count();
}

inline void ModelData::setStream(u32 index, Stream& stream)
{
	ZENIC_ASSERT(index < m_streams.count());
	m_streams[index] = stream;
}

inline ModelData::Stream& ModelData::stream(u32 index) const
{
	ZENIC_ASSERT(index < m_streams.count());
	return m_streams[index];
}

inline void ModelData::setIndexFormat(u32 indexFormat)
{
	m_indexFormat = indexFormat;
}

inline u32 ModelData::indexFormat() const
{
	return m_indexFormat;
}

inline void ModelData::setIndices(DataPtr<u8> indices)
{
	m_indices = indices;
}

inline DataPtr<u8> ModelData::indices() const
{
	return m_indices;
}

inline ModelData::Stream::Stream() : m_type(0), m_method(0), m_usage(0), m_index(0), m_size(0)
{
}

inline void ModelData::Stream::setType(u32 type)
{
	m_type = type;
}

inline u32 ModelData::Stream::type() const
{
	return m_type;
}

inline void ModelData::Stream::setMethod(u32 method)
{
	m_method = method;
}

inline u32 ModelData::Stream::method() const
{
	return m_method;
}

inline void ModelData::Stream::setUsage(u32 usage)
{
	m_usage = usage;
}

inline u32 ModelData::Stream::usage() const
{
	return m_usage;
}

inline void ModelData::Stream::setIndex(u32 index)
{
	m_index = index;
}

inline u32 ModelData::Stream::index() const
{
	return m_index;
}

inline void ModelData::Stream::setSize(u32 size)
{
	m_size = size;
}

inline u32 ModelData::Stream::size() const
{
	return m_size;
}

inline void ModelData::Stream::setData(DataPtr<u8> data)
{
	m_data = data;
}

inline DataPtr<u8> ModelData::Stream::data() const
{
	return m_data;
}
