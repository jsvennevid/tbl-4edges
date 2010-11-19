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

#include "DotModelBuilder.h"
#include <Shared/Geometry/Ps2/DotModel.h>
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

Model* DotModelBuilder::build(CSLModel* model)
{
	CSLXSIMesh* mesh = static_cast<CSLXSIMesh*>(model->Primitive());
	ZENIC_ASSERT(mesh);

	CSLXSIShape* shape = mesh->XSIShape();
	ZENIC_ASSERT(shape);

	// Positions

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

	DataPtr<s16> vertices;
	vertices.allocate(vertexCount * 3);
	s16* vertexArray = vertices.objects();

	for (uint i = 0; i < vertexCount; ++i)
	{
		const Vector3& pos = posVertexArray[i];
	
		vertexArray[0] = s16(pos.x * scale.x);
		vertexArray[1] = s16(pos.y * scale.y);
		vertexArray[2] = s16(pos.z * scale.z);
		vertexArray += 3;
	}

	DotModel* dotModel = zenic_new DotModel;

	dotModel->setScale(invScale);
	dotModel->setPoints(vertices);

	return dotModel;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

