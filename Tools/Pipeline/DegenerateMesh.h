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

#ifndef zenic_DegenerateMesh_h
#define zenic_DegenerateMesh_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSLXSIMesh;
class CSLXSIShape;
class CSLXSITriangleList;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Math/Vector3.h>
#include <Shared/Base/Math/Vector2.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DegenerateMesh
{
public:

	DegenerateMesh();
	~DegenerateMesh();

	struct Vertex
	{
		inline Vertex() : 
			position(0.0f, 0.0f, 0.0f), 
			normal(0.0f, 0.0f, 0.0f), 
			uv(0.0f, 0.0f), 
			nextIndex(0), 
			initialized(false)
		{
		}

		Vector3	position;
		Vector3 normal;
		Vector2 uv;
		u32 nextIndex;
		bool initialized;

		bool operator==(const Vertex& v) const;
	};

	void process(CSLXSIMesh* mesh, bool useNormals = false);
	const std::vector<u32>& indexList() const { return m_indices; }
	const std::vector<Vertex>& vertexList() const { return m_uniqueVertexList; }

private:

	u32 createOrRetrieveUniqueVertex(u32 originalPositionIndex, const Vertex& vertex);

	std::vector<Vertex> m_uniqueVertexList;
	std::vector<u32> m_indices;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
