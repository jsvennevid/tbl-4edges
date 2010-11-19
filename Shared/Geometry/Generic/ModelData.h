#ifndef zenic_generic_ModelData_h
#define zenic_generic_ModelData_h

#include "../ModelData.h"

#include "VertexMap.h"

#include <Shared/Base/Math/Vector3.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

namespace zenic
{

namespace generic
{

class ModelData : public zenic::ModelData
{
	ZENIC_SERIALIZABLE_FACTORY;
public:

	class Index
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:
		u32 m_index;
	};

	class Polygon
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:

		Polygon();
		~Polygon();

		void setNormal(const Vector3& normal);
		const Vector3& normal() const;

		void setIndices(DataPtr<u32> indices);
		DataPtr<u32> indices() const;

	private:

		Vector3 m_normal;
		DataPtr<Index> m_indices;
	};

	ModelData();
	virtual ~ModelData();

	virtual void serialize(Serializer& s);

	void setPolygons( DataPtr<Polygon> polygons );
	DataPtr<Polygon> polygons() const;

	void setVertexMap(VertexMap* VertexMap);
	VertexMap* vertexMap() const;
	VertexMap* vertexMap( VertexMap::Usage usage ) const;

private:

	DataPtr<Polygon> m_polygons;
	VertexMap* m_vertexMap;
};

#include "ModelData.inl"

}

}

#endif
