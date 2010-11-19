///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DegenerateMesh.h"
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Debug/Assert.h>

#include <XSIShape.h>
#include <XSIMesh.h>
#include <XSITriangleList.h>
#include <map>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DegenerateMesh::Vertex::operator == (const DegenerateMesh::Vertex& v) const
{
	return position == v.position && normal == v.normal && uv == v.uv;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DegenerateMesh::DegenerateMesh()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DegenerateMesh::~DegenerateMesh()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DegenerateMesh::process(CSLXSIMesh* mesh, bool useNormals)
{
	CSLXSIShape* shape = mesh->XSIShape();

	// We only handle one set of triangles for now.

	if (mesh->GetXSITriangleListCount() == 0)
	{
		ZENIC_WARNING("No triangles in the object. Can not be tristipped");
		return;
	}

	CSLXSITriangleList* triangleList = mesh->XSITriangleLists()[0];
	uint triCount = (uint)mesh->XSITriangleLists()[0]->GetTriangleCount() * 3;

	// Positions

	int* posIndexList = triangleList->GetVertexIndices()->ArrayPtr();
	Vector3* posVertexArray = (Vector3*)shape->GetVertexPositionList()->GetAttributeArray()->ArrayPtr();

	// Texture Coords

	int* uvIndexList = 0;
	Vector2* uvVertexArray = 0;

	int* normalIndexList = 0;
	Vector3* normalVertexArray = 0;

	CSLXSISubComponentAttributeList* componentList = shape->GetFirstTexCoordList();

	if (componentList)
	{
		uvIndexList = triangleList->GetAttributeByName(componentList->GetName())->ArrayPtr();
		uvVertexArray = (Vector2*)componentList->GetAttributeArray()->ArrayPtr();
	}

	componentList = shape->GetFirstNormalList();

	if (componentList && useNormals)
	{
		normalIndexList = triangleList->GetAttributeByName(componentList->GetName())->ArrayPtr();
		normalVertexArray = (Vector3*)componentList->GetAttributeArray()->ArrayPtr();
	}

	// setup the lists

	m_uniqueVertexList.clear();
	m_uniqueVertexList.resize(shape->GetVertexPositionList()->GetCount());
	m_indices.clear();
	m_indices.reserve(triCount); // intentionally reserved, not resized

	// generate the stuff

	for (uint i = 0; i < triCount; ++i)
	{
		Vertex vertex;

		vertex.position = posVertexArray[posIndexList[i]];
	
		if (uvVertexArray)	
			vertex.uv = uvVertexArray[uvIndexList[i]];

		if (normalVertexArray)	
			vertex.normal = normalVertexArray[normalIndexList[i]];

		u32 index = createOrRetrieveUniqueVertex(posIndexList[i], vertex);
		m_indices.push_back(index);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u32 DegenerateMesh::createOrRetrieveUniqueVertex(u32 originalPositionIndex, const Vertex& vertex)
{
    Vertex& orig = m_uniqueVertexList[originalPositionIndex];

    if (!orig.initialized)
    {
        orig = vertex;
        orig.initialized = true;
        return originalPositionIndex;
    }
    else if (orig == vertex)
    {
        return originalPositionIndex;
    }
    else
    {
        // no match, go to next or create new
        if (orig.nextIndex)
        {
            // cascade
            return createOrRetrieveUniqueVertex(orig.nextIndex, vertex);
        }
        else
        {
            // get new index

			u32 newIndex = u32(m_uniqueVertexList.size());
            orig.nextIndex = newIndex;

			m_uniqueVertexList.push_back(vertex);
            m_uniqueVertexList[newIndex].initialized = true;

            return newIndex;
        }
    }
}

}
