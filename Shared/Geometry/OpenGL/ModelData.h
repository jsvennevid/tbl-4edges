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

#ifndef zenic_opengl_ModelData_h
#define zenic_opengl_ModelData_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../ModelData.h"

#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

// TODA: UULY UGLY .. FIX FIX!

#include <Shared/Graphics/Renderer/OpenGL/Material.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Node;

	namespace opengl
	{
		class Material;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ModelData : public zenic::ModelData
{
	ZENIC_SERIALIZABLE_FACTORY;
public:

	ModelData();
	~ModelData();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Modeldata typ

	enum Type
	{
		Rigid,
		Skinned,
		Animated,
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Polygon Types

	enum
	{
		Triangels,
		TriStrips,
		Quads,
		QuadStrips
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// VertexFormat

	enum
	{
		Postion = 1,
		Color = 2,
		Normals = 4,
		Uv1 = 8,
		Uv2 = 16,
		Weight = 32,
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class PolygonList
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:

		void setPolygonType(u32 polygonType);
		void setVertexFormat(u32 vertexFormat);

		void setVertexStream(const DataPtr<f32>& data);
		void setIndexStream(const DataPtr<u32>& data);

		u32 polygonType() const;
		u32 vertexFormat() const;

		DataPtr<f32>& vertexStream();

	private:

		u32 m_polygonType;
		u32 m_vertexFormat;
		DataPtr<f32> m_vertexStream;
	};

	void setMaterial(Material* material);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Skin
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:
	
		void setAffector(Node* node);
		Node* affector() const;

		DataPtr<f32>& weightList();
		DataPtr<u32>& indexList();

	private:

		DataPtr<u32> m_indexList;
		DataPtr<f32> m_weightList;
		Node* m_affector;
	};

	virtual void serialize(Serializer& s);

	DataPtr<PolygonList>& polygons();
	void setPolygons(DataPtr<PolygonList>& polygonList);
	void setType(Type type);
	Type type() const;

	Material* material();

private:

	u16 m_type;

	DataPtr<PolygonList> m_polygons;
	DataPtr<Skin> m_skins;
	Material* m_material;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ModelData.inl"

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
