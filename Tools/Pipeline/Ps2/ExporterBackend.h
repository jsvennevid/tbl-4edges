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
