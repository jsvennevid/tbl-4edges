inline VertexMap::VertexMap() : m_type(u32(-1)), m_usage(u32(-1)), m_dimensions(0), m_next(0)
{
}

inline void VertexMap::setName(const String& name)
{
	m_name = name;
}

inline const String& VertexMap::name() const
{
	return m_name;
}

inline void VertexMap::setType(Type type)
{
	m_type = u32(type);
}

inline VertexMap::Type VertexMap::type() const
{
	return Type(m_type);
}

inline void VertexMap::setUsage(Usage usage)
{
	m_usage = u32(usage);
}

inline VertexMap::Usage VertexMap::usage() const
{
	return Usage(m_usage);
}

inline void VertexMap::setDimensions(u32 dimensions)
{
	m_dimensions = dimensions;
}

inline u32 VertexMap::dimensions() const
{
	return m_dimensions;
}

inline void VertexMap::setNext(VertexMap* next)
{
	m_next = next;
}

inline VertexMap* VertexMap::next() const
{
	return m_next;
}

inline void VertexMap::setIndices(DataPtr<u32> indices)
{
	m_indices = DataPtr<Index>(reinterpret_cast<Index*>(indices.objects()),indices.count(),indices.allocator());
}

inline DataPtr<u32> VertexMap::indices() const
{
	return DataPtr<u32>(reinterpret_cast<u32*>(m_indices.objects()),m_indices.count(),m_indices.allocator());
}

inline void VertexMap::setFloats(DataPtr<f32> floats)
{
	m_floats = DataPtr<FloatData>(reinterpret_cast<FloatData*>(floats.objects()),floats.count(),floats.allocator());
}

inline DataPtr<f32> VertexMap::floats() const
{
	return DataPtr<f32>(reinterpret_cast<f32*>(m_floats.objects()),m_floats.count(),m_floats.allocator());
}

inline void VertexMap::setIntegers(DataPtr<u32> integers)
{
	m_integers = DataPtr<IntegerData>(reinterpret_cast<IntegerData*>(integers.objects()),integers.count(),integers.allocator());
}

inline DataPtr<u32> VertexMap::integers() const
{
	return DataPtr<u32>(reinterpret_cast<u32*>(m_integers.objects()), m_integers.count(), m_integers.allocator());
}
