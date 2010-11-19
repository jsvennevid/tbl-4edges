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

#include "BranchModelBuilder.h"
#include <Shared/Geometry/Ps2/BranchModel.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Memory/New.h>

#include <XSIShape.h>
#include <XSIMesh.h>
#include <XSITriangleList.h>
#include <XSIPolygonList.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Model* BranchModelBuilder::build(CSLModel* model)
{
	CSLXSIMesh* mesh = static_cast<CSLXSIMesh*>(model->Primitive());
	ZENIC_ASSERT(mesh);

	CSLXSIShape* shape = mesh->XSIShape();
	ZENIC_ASSERT(shape);

	CSLXSIPolygonList* polygonList = mesh->XSIPolygonLists()[0];
	ZENIC_ASSERT(polygonList);
	const uint* nodeCountArray = reinterpret_cast<const uint*>(polygonList->GetPolygonNodeCountArray()->ArrayPtr());
	uint polyCount = (uint)polygonList->GetPolygonCount();

	// Positions

	const uint* posIndexList = reinterpret_cast<const uint*>(polygonList->GetVertexIndices()->ArrayPtr());
	ZENIC_ASSERT(posIndexList);

	const Vector3* posVertexArray = (Vector3*)shape->GetVertexPositionList()->GetAttributeArray()->ArrayPtr();
	ZENIC_ASSERT(posVertexArray);
	uint vertexCount = shape->GetVertexPositionList()->GetCount();

	// Find the scale of the model

	Vector3 minPos(100000.0f, 100000.0f, 100000.f);
	Vector3 maxPos(-100000.0f, -100000.0f, -100000.f);

	uint startId = 0;

	for (uint i = 0; i < vertexCount; ++i)
	{
		const Vector3& pos = posVertexArray[i];
	
		if (pos.x > maxPos.x)
			maxPos.x = pos.x;

		if (pos.y > maxPos.y)
			maxPos.y = pos.y;

		if (pos.z > maxPos.z)
			maxPos.z = pos.z;

		if (pos.x < minPos.x)
			minPos.x = pos.x;

		if (pos.y < minPos.y)
			minPos.y = pos.y;

		if (pos.z < minPos.z)
			minPos.z = pos.z;
	}

	Vector3 scale(32767.0f / (abs(minPos.x) + abs(maxPos.x)), 
				  32767.0f / (abs(minPos.y) + abs(maxPos.y)), 
				  32767.0f / (abs(minPos.z) + abs(maxPos.z)));

	Vector3 invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);

	// Calculate the total number of coords

	uint totalCount = 0;

	for (uint i = 0; i < polyCount; ++i)
		totalCount += nodeCountArray[i];

	DataPtr<BranchModel::Vertex> curves;
	curves.allocate(totalCount);
	BranchModel::Vertex* vertexArray = curves.objects();

	DataPtr<u16> curvesCount;
	curvesCount.allocate(polyCount);
	u16* curvesCountArray = curvesCount.objects();

	for (uint i = 0; i < polyCount; ++i)
	{
		curvesCountArray[i] = u16(nodeCountArray[i]);			

		for (uint k = 0; k < nodeCountArray[i]; k++)
		{
			//Vector3 pos = posVertexArray[*posIndexList++];
			Vector3 pos = *posVertexArray++;
			pos.x *= scale.x;
			pos.y *= scale.y;
			pos.z *= scale.z;

			vertexArray[0].x = (s16)pos.x;
			vertexArray[0].y = (s16)pos.y;
			vertexArray[0].z = (s16)pos.z;
			++vertexArray;
		}
	}

	BranchModel* branchModel = zenic_new BranchModel;

	branchModel->setCurves(curves);
	branchModel->setCurvesCount(curvesCount);
	branchModel->setScale(invScale);

	return branchModel;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

