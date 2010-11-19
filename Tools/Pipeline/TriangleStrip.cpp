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

#include "TriangleStrip.h"
#include "TriangleStripActc.h"
#include "DegenerateMesh.h"
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Geometry/Generic/ModelData.h>

#pragma warning(disable : 4505 4100 4996 4244)

#include <windows.h>
#include <XSIMesh.h>
#include <XSITriangleList.h>
#include <XSIVertexList.h>

#pragma warning(default : 4100 4996 4244)  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TriangleStrip::TriangleStrip(TriangleStrip::Technique technique) : m_technique(technique)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TriangleStrip::~TriangleStrip()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TriangleStrip::StripInfo TriangleStrip::process(DegenerateMesh* mesh)
{
	StripInfo stripInfo;

	const std::vector<u32>& indices = mesh->indexList();

	int* indexList = zenic_new int[indices.size()];

	stripInfo.remapTable = 0;
	stripInfo.stripList.clear();

	for (uint i = 0; i < indices.size(); ++i)
		indexList[i] = indices[i];

	switch (m_technique)
	{
		case Actc : return processActc(indexList, (int)indices.size()); break;
		default :
		{
			ZENIC_ERROR("Tristriping Technique not supported");
			break;		
		}
	}

	zenic_delete [] indexList;

	return stripInfo;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TriangleStrip::StripInfo TriangleStrip::process(CSLXSIMesh* mesh)
{
	StripInfo stripInfo;
	int* indexList = 0;
	int indexListCount = 0;

	stripInfo.remapTable = 0;
	stripInfo.stripList.clear();

	//CSLXSIVertexList* vertexList = mesh->XSIVertexList();
	//indexList = vertexList->GetAttributeByName("position")->ArrayPtr();
	//indexListCount = vertexList->GetCount();

	if (mesh->GetXSITriangleListCount() == 0)
	{
		ZENIC_WARNING("No triangles in the object. Can not be tristipped");
		return stripInfo;
	}

	indexList = mesh->XSITriangleLists()[0]->GetVertexIndices()->ArrayPtr();
	indexListCount = mesh->XSITriangleLists()[0]->GetTriangleCount() * 3;

	switch (m_technique)
	{
		case Actc : return processActc(indexList, indexListCount); break;
		default :
		{
			ZENIC_ERROR("Tristriping Technique not supported");
			break;		
		}
	}


	return stripInfo;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TriangleStrip::StripInfo TriangleStrip::processActc(const int* indexList, const int indexListCount)
{
	StripInfo stripInfo;

	int prim = 0;
	uint v1 = 0;
	uint v2 = 0;
	uint v3 = 0;

	actc::ACTCData* tc = actc::actcNew();
	ZENIC_ASSERT(tc);

	actc::actcBeginInput(tc);

	const int* indexListTemp = indexList;

	for (int i = 0; i < indexListCount/3; ++i)
	{
		actc::actcAddTriangle(tc, indexListTemp[0], indexListTemp[1], indexListTemp[2]);
		indexListTemp += 3;
	}

	actc::actcEndInput(tc);
	actc::actcBeginOutput(tc);

	while ((prim = actcStartNextPrim(tc, &v1, &v2)) != ACTC_DATABASE_EMPTY) 
	{
		std::vector<uint> tempList;

		tempList.clear();
		tempList.push_back(v1);
		tempList.push_back(v2);
				
	    while (actcGetNextVert(tc, &v3) != ACTC_PRIM_COMPLETE) 
			tempList.push_back(v3);

		DataPtr<u32>* strip = zenic_new DataPtr<u32>;

		strip->allocate(u32(tempList.size()));
		u32* objects = strip->objects();

		for (uint i = 0; i < tempList.size(); ++i)
			objects[i] = tempList[i];
				
		stripInfo.stripList.push_back(strip);
	}

	// Build remap table for the other lists (normals, vus, etc can use)

	stripInfo.remapTable = zenic_new u32[indexListCount];

	//for (int i = indexListCount; i > 0; i--)
	//	stripInfo.remapTable[indexList[i-1]] = i-1;

	for (int i = 0; i < indexListCount; ++i)
		stripInfo.remapTable[indexList[i]] = i;

	return stripInfo;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
