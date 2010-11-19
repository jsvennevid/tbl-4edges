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

#ifndef zenic_PostEffectHelper_h
#define zenic_PostEffectHelper_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

#define POSTEFFECTHELPER_USE_SPR

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Renderer;
		class Dma;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PostEffectHelper
{
public:

	PostEffectHelper(Renderer& renderer, Dma& dmaChain);
	~PostEffectHelper();

	enum
	{
		Width = 512,
		Height = 448,
		Tw = 9,
		Th = 9,
		Fbw = 8,
	};


	void glareOffScreen(uint screen, float cutoff, int depth, float sourceScale, float gain, float clampBlack = 0.5f);
	void glare(float cutoff, int depth, float sourceScale, float gain, float clampBlack = 0.5f);
	void blur(int bp0, int bp1, int bw, int w, int h, int depth, float gain, float clampBlack);
	void fadeFrame(uint r, uint g, uint b, float strength);
	void fadeDownFrame(uint r, uint g, uint b, float strength);

	void motionBlur(u32 strength);
	void setAlphaAddFix(const uint& fix = 0x80);
	void setAlphaSubFix(const uint& fix = 0x80);
	void setAlpha(int a, int b, int c, int d, int fix);
	void setClamp(int wms, int wmt, int minu, int maxi,
				  int minv, int maxv);
	void setClamp(int w, int h);
	void setFrame();
	void setFrame(int fbp, int fbw, int psm = 0, int fbmsk = 0);
	void setRgba(int r, int g, int b, int a);
	void setScissor(int scax0, int scay0, int scax1, int scay1);
	void setTest(int ate, int atst, int aref, int afail,
						int date, int datm, int zte, int ztst);

	void setTex0(u64 texReg);
	void setTex0();
	void setTex0(int tbp0, int tbw, int psm = 0, int tw = Tw,
						int th = Th, int tcc = 0, int tfx = 0);
	void setTex0Pal(int tbp0, int cbp, int tbw, int psm = 0, int tw = Tw,
					int th = Th, int tcc = 0, int tfx = 0);
	void setFiltering(int filtering);
	void setTexflush();
	void setXyoffset(int ofx, int ofy);
	void flushRegisters();

	void blit(int x0, int y0, int x1, int y1, bool alpha = false,
			  bool texture = false, bool flip = false);
	void blit(int x0, int y0, int x1, int y1,
			  float u0, float v0, float u1, float v1,
			  bool alpha = false, bool texture = true, bool flip = false);
	void blit(float fx0, float fy0, float fx1, float y1,
			  float u0, float v0, float u1, float v1,
			  bool alpha = false, bool texture = true, bool flip = false);
	void downSample(int x0, int y0, int x1, int y1, bool alpha = false);
	void upSample(int x0, int y0, int x1, int y1, bool alpha = false);

	void drawBox(int x0, int y0, int x1, int y1, bool alpha = false);

private:

	struct blitVertex
	{
		float u;
		float v;
		u64   fill0;
		int   x;	
		int   y;	
		u64   fill1;
	};

	enum REGS 
	{
		ALPHA,
		CLAMP,
		FRAME,
		RGBAQ,
		SCISSOR,
		TEST,
		TEX0,
		TEXFLUSH,
		XYOFFSET,
		TEX1,
		NUM_REGS
	};

	static const int m_regAddress[NUM_REGS];
	Renderer& m_renderer;
	Dma& m_dmaChain;
	uint m_textureWidth;
	uint m_textureHeight;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif //zenic_PostEffectHelper_h

