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
