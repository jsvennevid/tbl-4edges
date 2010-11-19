/*

Copyright (c) 2004-2006 Jesper Svennevid, Daniel Collin

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
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
