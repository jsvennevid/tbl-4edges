///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_Ps2DebugRenderer_h
#define zenic_Ps2DebugRenderer_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../DebugRenderer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Camera;
		class Dma;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DebugRenderer : public zenic::DebugRenderer
{
public:

	DebugRenderer(Dma& dmaChain, Camera& camera);
	virtual ~DebugRenderer();

	void text(const char* dispText, float x, float y, const Color32& color);
	void triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color32& color);
	void line(const Vector3& p0, const Vector3& p1, const Color32& color);
	void begin();
	void end();

private:

	void triangleInternal(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color32& color, uint primType = 0);
	u64 setTex0Pal(int tbp0, int cbp, int tbw, int psm, int tw, int th, int tcc, int tfx);
	void findRow(char c, float& s, float& t);
	void initImage();
	static u8 m_fontTexture[];

	Dma& m_dmaChain;
	Camera& m_camera;

	s32 m_vuProgram;

	u8* m_callChains[2];
	u8* m_imageData;	
	u8* m_palData;

	bool m_begin;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif


