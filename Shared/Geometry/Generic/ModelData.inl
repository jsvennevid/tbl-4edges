inline ModelData::ModelData() : m_vertexMap(0)
{
}

inline void ModelData::setPolygons( DataPtr<Polygon> polygons )
{
	m_polygons = polygons;
}

inline DataPtr<ModelData::Polygon> ModelData::polygons() const
{
	return m_polygons;
}

inline void ModelData::setVertexMap(VertexMap* vertexMap)
{
	m_vertexMap = vertexMap;
}

inline VertexMap* ModelData::vertexMap() const
{
	return m_vertexMap;
}

inline VertexMap* ModelData::vertexMap( VertexMap::Usage usage ) const
{
	for (VertexMap* current = m_vertexMap; current; current = current->next())
	{
		if( current->usage() == usage )
			return current;
	}

	return 0;
}

inline ModelData::Polygon::Polygon() : m_normal(0,0,0)
{
}

inline ModelData::Polygon::~Polygon()
{
	m_indices.free();
}

inline void ModelData::Polygon::setNormal(const Vector3& normal)
{
	m_normal = normal;
}

inline const Vector3& ModelData::Polygon::normal() const
{
	return m_normal;
}

inline void ModelData::Polygon::setIndices(DataPtr<u32> indices)
{
	m_indices = DataPtr<Index>(reinterpret_cast<Index*>(indices.objects()),indices.count(),indices.allocator());
}

inline DataPtr<u32> ModelData::Polygon::indices() const
{
	return DataPtr<u32>(reinterpret_cast<u32*>(m_indices.objects()),m_indices.count(),m_indices.allocator());
}
