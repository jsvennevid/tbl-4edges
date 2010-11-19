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

#ifndef zenic_ps2_ExporterBackend_h
#define zenic_ps2_ExporterBackend_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSLXSIShape;
class CSLXSIMaterial;

namespace zenic
{
	class String;

	namespace ps2
	{
		class ModelData;
		class Texture;
		class Dma;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../ExporterBackend.h"
#include <Shared/Base/Types.h>
#include <Shared/Base/Math/Vector3.h>
#include <Shared/Base/Math/Color32.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ExporterBackend : public zenic::ExporterBackend
{
public :

	ExporterBackend() {}
	~ExporterBackend() {}

	zenic::ModelData* buildModelData(CSLModel* model, zenic::Model* modelNode);

	zenic::Model* buildModel(CSLModel* mesh);
	zenic::Material* buildMaterial(CSLXSIMaterial* xsiMaterial);
	Serializable* buildTexture(CSLImage* image, CSIBCPixMap& pixMap);

private:

	typedef struct
	{
		Vector3 vertex;
		Color32 color;
		Vector3 normal;
		float s;
		float t;
		uint adc;

	} MeshVertex;

	enum VertexFlags
	{
		Position = 1,
		Color = 2,
		Normal = 4,
		Texcoord = 8,
		PositionInt = 16
	};

	enum
	{
		Vu1ChunkSize = 490,		// usually about half size of vu-memory
		Vu0ChunkSize = 128
	};

	void calculateBound(CSLXSIShape* shape, zenic::Model* modelNode);
	void setupVertexFormat(CSLXSIShape* shape);
	void setupShaderPipeline(ps2::ModelData* modelData, const char* name);

	void flushChunk(Dma& dmaChain);
	void addVertex(Dma& dmaChain, const Vector3& vertex, const Color32& color, const Vector3& normal,
				   const float& s, const float& t, const uint& adc);

	void getChain(ModelData* target, Dma& dmaChain);

	u32 m_chunkCount;
	u32 m_chunkSize;
	u32 m_cycleFormat;
	u32 m_vertexFormat;
	u32 m_vertexOffset;
	Vector3 m_maxScale;

	static MeshVertex m_buildChunk[256];
	static uint m_vu1HeaderSize;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif
