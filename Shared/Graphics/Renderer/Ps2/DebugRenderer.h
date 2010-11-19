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


