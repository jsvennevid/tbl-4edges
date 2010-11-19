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

#include "PostEffectHelper.h"

#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Memory/Prefetch.h>

#include <stdio.h>
#ifndef PS2_EMU
#include <kernel.h>
#endif
#include <string.h>

#ifdef PS2_EMU

u64 m_registers[16];
bool m_registerChanged[16];

#else

u64* m_registers = 0; //[NUM_REGS];
bool* m_registerChanged = 0; //[NUM_REGS];


#endif

//#define INLINE_T inline

#define INLINE_T

#ifdef PS2_EMU
#undef POSTEFFECTHELPER_USE_SPR
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

//inline void pref(const void* ptr) { asm __volatile__("pref   0,0(%0)" : : "r" (ptr)); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int PostEffectHelper::m_regAddress [PostEffectHelper::NUM_REGS] = 
{
	0x42,
	0x08,
	0x4c,
	0x01,
	0x40,
	0x47,
	0x06,
	0x3f,
	0x18,
	0x14,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PostEffectHelper::PostEffectHelper(Renderer& renderer, Dma& dmaChain) :
	m_renderer(renderer),
	m_dmaChain(dmaChain),
	m_textureWidth(0),
	m_textureHeight(0)
{
#ifndef PS2_EMU
	m_registers = (u64*)((char*)0x70003800);
	m_registerChanged = (bool*)((char*)0x70003800) + sizeof(u64) * NUM_REGS;
#endif

	m_dmaChain.addSrcCntTag();
	m_dmaChain.add32(VIF_FLUSHA());
	m_dmaChain.add32(VIF_FLUSHA());
	m_dmaChain.add32(VIF_FLUSHE());
	m_dmaChain.add32(0);
	m_dmaChain.endPacket();

	setAlpha(0, 1, 0, 1, 0);
	setClamp(0, 0, 0, 0, 0, 0);
	setFrame(m_renderer.currentScreen(), Fbw, 0, 0);
	setRgba(128, 128, 128, 128);
	setScissor(0, 0, Width-1, Height-1);
	setTest(1, 0, 0, 1, 0, 0, 1, 1);
	setTex0(m_renderer.currentScreen(), Fbw, 0, Tw, Th, 1, 0);
	setTexflush();
	setFiltering(1);
	setXyoffset(0, 0);

	flushRegisters();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PostEffectHelper::~PostEffectHelper()
{
	setAlpha(0, 1, 0, 1, 0);
	setClamp(0, 0, 0, 0, 0, 0);
	flushRegisters();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setAlpha(int a, int b, int c, int d, int fix)
{
	m_registerChanged[ALPHA] = true;
	m_registers[ALPHA] = a + (b << 2) + (c << 4) + (d << 6) + ((u64)fix << 32);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setAlphaAddFix(const uint& fix)
{
	setAlpha(0, 2, 2, 1, fix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setAlphaSubFix(const uint& fix)
{
	setAlpha(1, 0, 2, 2, fix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setClamp(int wms, int wmt, int minu, int maxu, int minv, int maxv)
{
	m_registerChanged[CLAMP] = true;
	m_registers[CLAMP] = wms + (wmt << 2) + (minu << 4) + (maxu << 14) +((u64)minv << 24) + ((u64)maxv << 34);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setClamp(int w, int h)
{
	setClamp(2, 2, 0, w-1, 0, h-1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setFrame()
{
	setFrame(m_renderer.currentScreen(), Fbw);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setFrame(int fbp, int fbw, int psm, int fbmsk)
{
	m_registerChanged[FRAME] = true;
	m_registers[FRAME] = fbp + (fbw << 16) + (psm << 24) + ((u64)fbmsk << 32);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setRgba(int r, int g, int b, int a)
{
	m_registerChanged[RGBAQ] = true;
	m_registers[RGBAQ] = r + (g << 8) + (b << 16) + ((u64)a << 24) + ((u64)0x3f800000 << 32);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setScissor(int scax0, int scay0, int scax1, int scay1)
{
	m_registerChanged[SCISSOR] = true;
	m_registers[SCISSOR] = scax0 + (scax1 << 16) + ((u64)scay0 << 32) + ((u64)scay1 << 48);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setTest(int ate, int atst, int aref, int afail, int date, int datm, int zte, int ztst)
{
	m_registerChanged[TEST] = true;
	m_registers[TEST] = ate + (atst << 1) + (aref << 4) + (afail << 12) +
					  (date << 14) + (datm << 15) + (zte << 16) + (ztst << 17);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setTex0()
{
	setTex0(m_renderer.currentScreen(), 8);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setTex0(u64 value)
{
	m_registerChanged[TEX0] = true;
	m_registers[TEX0] = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setTex0(int tbp0, int tbw, int psm, int tw,
		int th, int tcc, int tfx)
{
	m_registerChanged[TEX0] = true;
	m_registers[TEX0] = tbp0*32 + (tbw << 14) + (psm << 20) + (tw << 26) + ((u64)th << 30) + 
		                          ((u64)tcc << 34) + ((u64)tfx << 35);
	m_textureWidth = 1 << tw;
	m_textureHeight = 1 << th;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::setTex0Pal(int tbp0, int cbp, int tbw, int psm, int tw, int th, int tcc, int tfx)
{
	m_registerChanged[TEX0] = true;
	m_registers[TEX0] = tbp0 + (tbw << 14) + (psm << 20) + (tw << 26) + ((u64)th << 30) + ((u64)tcc << 34) + 
							   ((u64)tfx << 35) + ((u64)cbp << 37) + ((u64)0 << 55);
	m_textureWidth = 1 << tw;
	m_textureHeight = 1 << th;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setFiltering(int filtering)
{
	m_registerChanged[TEX1] = true;
	m_registers[TEX1] = (filtering << 5) + (filtering << 6);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setTexflush()
{
	m_registerChanged[TEXFLUSH] = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE_T void PostEffectHelper::setXyoffset(int ofx, int ofy)
{
	m_registerChanged[XYOFFSET] = true;
	m_registers[XYOFFSET] = ofx + ((u64)ofy << 32);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::flushRegisters()
{
	uint changedRegsCount = 0;

	// not very nice but..	

	ZENIC_PREFETCH_WRITE(m_dmaChain.currentLocation());

	for (uint i = 0; i < NUM_REGS; ++i)
	{
		if (m_registerChanged[i])
		  ++changedRegsCount;
	}

	m_dmaChain.addSrcCntTag(0, VIF_DIRECT(changedRegsCount+1));
	m_dmaChain.add128(GIFTAG(changedRegsCount, gif::Eop, 0, 0, gif::Packed, 1 ), gif::A_D);

	for (uint i = 0; i < NUM_REGS; ++i)
	{
		if (m_registerChanged[i])
		{
			m_dmaChain.add64(m_registers[i]);
			m_dmaChain.add64(m_regAddress[i]);

			ZENIC_PREFETCH_WRITE(((char*)m_dmaChain.currentLocation()) + 64);

			m_registerChanged[i] = false;
		}
	}

	m_dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::blit(int x0, int y0, int x1, int y1, bool alpha, bool texture, bool flip)
{
	blit(x0, y0, x1, y1, x0 + 0.5f, y0 + 0.5f, x1 + 0.5f, y1 + 0.5f, alpha, texture, flip);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::blit(int x0, int y0, int x1, int y1, float u0, float v0, float u1, float v1, bool alpha,
							bool texture, bool flip)
{
	float ud, maxu;
	int nstrips, xstep, maxx;

	x0 <<= 4;
	y0 <<= 4;

	x1 <<= 4;
	y1 <<= 4;

	if (flip)
	{
		float w, h;
		w = u1 - u0;
		h = v1 - v0;
		u0 = u1 - w / (x1 - x0);
		v0 = v1 - h / (y1 - y0);
		u1 = u0 - w;
		v1 = v0 - h;
	}

	ud = (u0 < u1) ? u1 - u0 : u0 - u1;
	nstrips = (int)(ud + 32) / 32;
	xstep = (x1 - x0 + nstrips - 1) / nstrips;
	ud = (u1 - u0) * xstep / (x1 - x0);
	
	maxx = x1;
	maxu = u1 / m_textureWidth;
	x1 = x0 + xstep;
	u1 = u0 + ud;

	u0 /= m_textureWidth;
	v0 /= m_textureHeight; 
	
	u1 /= m_textureWidth;
	v1 /= m_textureHeight; 
	
	ud /= m_textureWidth;

	// TODO: Put this on SPR as well?

	flushRegisters();

#ifdef POSTEFFECTHELPER_USE_SPR

	void* dmaTarget = m_dmaChain.reserveEnd(((nstrips * 64) + 2 * 8) * 2);
	m_dmaChain.addSrcCallTag(dmaTarget);

	ZENIC_PREFETCH_WRITE(m_dmaChain.currentLocation());

	//Dma::waitFromSp();

	Dma sprChain((void*)0x70002000, 4096);

	sprChain.addSrcRetTag(0,VIF_DIRECT((nstrips*4)+1));
	sprChain.add64(GIFTAG(nstrips, gif::Eop, gif::Pre, 6 + (alpha ? 64 : 0) + (texture ? 16 : 0), gif::Packed, 4));
	sprChain.add64(0x5252);

	blitVertex* dmaData = static_cast<blitVertex*>(sprChain.reserve(nstrips * 64));
	sprChain.endPacket();

#else

	m_dmaChain.addSrcCntTag(0,VIF_DIRECT((nstrips*4)+1));
	m_dmaChain.add64(GIFTAG(nstrips, gif::Eop, gif::Pre, 6 + (alpha ? 64 : 0) + (texture ? 16 : 0), gif::Packed, 4));
	m_dmaChain.add64(0x5252);

	blitVertex* dmaData = static_cast<blitVertex*>(m_dmaChain.reserve(nstrips * 64));
	m_dmaChain.endPacket();

#endif

	for (int i = 0; i < nstrips; ++i)
	{
		dmaData[0].u = u0;
		dmaData[0].v = v0;
		dmaData[0].fill0 = 0;
		dmaData[0].x = x0;
		dmaData[0].y = y0;
		dmaData[0].fill1 = 0;

		dmaData[1].u = u1;
		dmaData[1].v = v1;
		dmaData[1].fill0 = 0;
		dmaData[1].x = x1;
		dmaData[1].y = y1;
		dmaData[1].fill1 = 0;

		dmaData += 2;

		x0 = x1; x1 += xstep;
		u0 = u1; u1 += ud;

		if (x1 > maxx)
		{
			x1 = maxx;
			u1 = maxu;
		}
	}

#ifdef POSTEFFECTHELPER_USE_SPR
	Dma::sendNormalFromSp(dmaTarget, (void*)0x70002000, (nstrips * 64/16) + 2); 
#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::blit(float fx0, float fy0, float fx1, float fy1, float u0, float v0, float u1, float v1, bool alpha,
							bool texture, bool flip)
{
	float ud, maxu;
	int nstrips, xstep, maxx;

	int x0 = int(fx0 * 16.0f);
	int y0 = int(fy0 * 16.0f);

	int x1 = int(fx1 * 16.0f);
	int y1 = int(fy1 * 16.0f);

	if (flip)
	{
		float w, h;
		w = u1 - u0;
		h = v1 - v0;
		u0 = u1 - w / (x1 - x0);
		v0 = v1 - h / (y1 - y0);
		u1 = u0 - w;
		v1 = v0 - h;
	}

	ud = (u0 < u1) ? u1 - u0 : u0 - u1;
	nstrips = (int)(ud + 32) / 32;
	xstep = (x1 - x0 + nstrips - 1) / nstrips;
	ud = (u1 - u0) * xstep / (x1 - x0);
	
	maxx = x1;
	maxu = u1 / m_textureWidth;
	x1 = x0 + xstep;
	u1 = u0 + ud;

	u0 /= m_textureWidth;
	v0 /= m_textureHeight; 
	
	u1 /= m_textureWidth;
	v1 /= m_textureHeight; 
	
	ud /= m_textureWidth;

	// TODO: Put this on SPR as well?

	flushRegisters();

#ifdef POSTEFFECTHELPER_USE_SPR

	void* dmaTarget = m_dmaChain.reserveEnd(((nstrips * 64) + 2 * 8) * 2);
	m_dmaChain.addSrcCallTag(dmaTarget);

	ZENIC_PREFETCH_WRITE(m_dmaChain.currentLocation());

	//Dma::waitFromSp();

	Dma sprChain((void*)0x70002000, 4096);

	sprChain.addSrcRetTag(0,VIF_DIRECT((nstrips*4)+1));
	sprChain.add64(GIFTAG(nstrips, gif::Eop, gif::Pre, 6 + (alpha ? 64 : 0) + (texture ? 16 : 0), gif::Packed, 4));
	sprChain.add64(0x5252);

	blitVertex* dmaData = static_cast<blitVertex*>(sprChain.reserve(nstrips * 64));
	sprChain.endPacket();

#else

	m_dmaChain.addSrcCntTag(0,VIF_DIRECT((nstrips*4)+1));
	m_dmaChain.add64(GIFTAG(nstrips, gif::Eop, gif::Pre, 6 + (alpha ? 64 : 0) + (texture ? 16 : 0), gif::Packed, 4));
	m_dmaChain.add64(0x5252);

	blitVertex* dmaData = static_cast<blitVertex*>(m_dmaChain.reserve(nstrips * 64));
	m_dmaChain.endPacket();

#endif

	for (int i = 0; i < nstrips; ++i)
	{
		dmaData[0].u = u0;
		dmaData[0].v = v0;
		dmaData[0].fill0 = 0;
		dmaData[0].x = x0;
		dmaData[0].y = y0;
		dmaData[0].fill1 = 0;

		dmaData[1].u = u1;
		dmaData[1].v = v1;
		dmaData[1].fill0 = 0;
		dmaData[1].x = x1;
		dmaData[1].y = y1;
		dmaData[1].fill1 = 0;

		dmaData += 2;

		x0 = x1; x1 += xstep;
		u0 = u1; u1 += ud;

		if (x1 > maxx)
		{
			x1 = maxx;
			u1 = maxu;
		}
	}

#ifdef POSTEFFECTHELPER_USE_SPR
	Dma::sendNormalFromSp(dmaTarget, (void*)0x70002000, (nstrips * 64/16) + 2); 
#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::downSample(int x0, int y0, int x1, int y1, bool alpha)
{
	blit(x0, y0, x1, y1, x0*2+1, y0*2+1, x1*2+1, y1*2+1, alpha);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::upSample(int x0, int y0, int x1, int y1, bool alpha)
{
	blit(x0, y0, x1, y1, (float)x0/2, (float)y0/2, (float)x1/2, (float)y1/2, alpha);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::drawBox(int x0, int y0, int x1, int y1, bool alpha)
{
	flushRegisters();

	m_dmaChain.addSrcCntTag(0,VIF_DIRECT(6));
	m_dmaChain.add64(GIFTAG(5, gif::Eop, gif::Pre, 2 + (alpha ? 64 : 0), gif::Packed, 1));
	m_dmaChain.add64(gif::XYZ2);

	m_dmaChain.add32(x0 * 16); m_dmaChain.add32(y0 * 16); m_dmaChain.add64(0);
	m_dmaChain.add32(x1 * 16); m_dmaChain.add32(y0 * 16); m_dmaChain.add64(0);
	ZENIC_PREFETCH_WRITE(((char*)m_dmaChain.currentLocation()) + 64);
	m_dmaChain.add32(x1 * 16); m_dmaChain.add32(y1 * 16); m_dmaChain.add64(0);
	m_dmaChain.add32(x0 * 16); m_dmaChain.add32(y1 * 16); m_dmaChain.add64(0);
	m_dmaChain.add32(x0 * 16); m_dmaChain.add32(y0 * 16); m_dmaChain.add64(0);
	ZENIC_PREFETCH_WRITE(((char*)m_dmaChain.currentLocation()) + 64);
	m_dmaChain.endPacket();
}
/*
void PostEffectHelper::glareOffScreen(uint screen,float cutoff, int depth, float sourceScale, float gain, float clampBlack)
{
	setTex0(screen, 8, 0, 9, 9, 1, 0);
	glare(cutoff,depth,sourceScale,gain,clampBlack);
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::glareOffScreen(uint screen,float cutoff, int depth, float sourceScale, float gain, float clampBlack)
{
	setTex0(screen, 8, 0, 9, 9, 1, 0);

	int offset = 0;

	setFrame(450, Fbw/2);
	downSample(0, 0, Width/2, Height/2);

	blur(450, screen, Fbw/2, Width/2, Height/2, depth, gain, 0.5f);

	if (cutoff > 0)
	{
		setAlpha(2, 0, 2, 1, u32(cutoff * 256));
		blit(0, 0, Width/2, Height/2, true);
	}

	blur(450, screen, Fbw/2, Width/2, Height/2, depth, gain, 0.5f);

	setTexflush();
	setFrame();
	setTex0(450, Fbw/2);
	setClamp(2, 2, 0, (Width/2) - 1, 0, (Height/2) - 1);

	if (sourceScale > 0)
		setAlpha(1, 2, 2, 0, u32(sourceScale * 128.0f));
	else
		setAlpha(2, 1, 2, 0, -u32(sourceScale * 128.0f));

	offset = (depth & 1) ? (1 << (depth-2)) : -(1 << (depth-2));
	blit(0, 0, Width, Height, 4, 4, (Width/2) - 4 , (Height/2) - 4, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::glare(float cutoff, int depth, float sourceScale, float gain, float clampBlack)
{
	int offset = 0;

	setFrame(224, Fbw/2);
	downSample(0, 0, Width/2, Height/2);

	blur(224, 280, Fbw/2, Width/2, Height/2, depth, gain, 0.5f);

	if (cutoff > 0)
	{
		setAlpha(2, 0, 2, 1, u32(cutoff * 256));
		blit(0, 0, Width/2, Height/2, true);
	}

	blur(224, 280, Fbw/2, Width/2, Height/2, depth, gain, 0.5f);

	setTexflush();
	setFrame();
	setTex0(224, Fbw/2);
	setClamp(2, 2, 0, (Width/2) - 1, 0, (Height/2) - 1);

	if (sourceScale > 0)
		setAlpha(1, 2, 2, 0, u32(sourceScale * 128.0f));
	else
		setAlpha(2, 1, 2, 0, -u32(sourceScale * 128.0f));

	offset = (depth & 1) ? (1 << (depth-2)) : -(1 << (depth-2));
	blit(0, 0, Width, Height, 4, 4, (Width/2) - 4 , (Height/2) - 4, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::blur(int bp0, int bp1, int bw, int w, int h, int depth, float gain, float clampBlack)
{
	int hw = (bw & 1) ? (bw+1)/2 : bw / 2;

	setClamp(2, 2, 0, w-1, 0, h-1);
	setFrame(bp0, bw);
	setTex0(bp0, bw);

	if (clampBlack > 0)
	{
		setAlpha(2, 1, 2, 1, (int)(clampBlack * 128.0f));
		drawBox(0, 0, w-1, h-1, true);
		setTexflush();
	}

	setAlpha(1, 0, 2, 0, 32);
	blit(0, 0, w, h, 1.8333f, 0.5f, w+1.8333f, h+0.5f, true);
	setTexflush();
	blit(0, 0, w, h, 0.5f, 1.8333f, w+0.5f, h+1.8333f, true);
	setTexflush();

	setFrame(bp1, hw);
	setRgba((int)(gain * 128.0f), (int)(gain * 128.0f), (int)(gain * 128.0f), 128);
	blit(0, 0, w/2, h/2, 1, 1, w+1, h+1, false, true, true);
	setTexflush();
	setRgba(128, 128, 128, 128);

	if (depth > 1)
		blur(bp1, bp0, hw, w/2, h/2, depth-1, gain, clampBlack);

	setClamp(2, 2, 0, w/2-1, 0, h/2-1);
	setFrame(bp1, hw);
	setTex0(bp1, hw);

	if (clampBlack > 0)
	{
		setAlpha(2, 1, 2, 1, (int)(clampBlack * 128.0f));
		drawBox(0, 0, w/2-1, h/2-1, true);
		setAlpha(1, 0, 2, 0, 32);
		setTexflush();
	}

	blit(0, 0, w/2, h/2, 1.8333f, 0.5f, w/2+1.8333f, h/2+0.5f, true);
	setTexflush();
	blit(0, 0, w/2, h/2, 0.5f, 1.8333f, w/2+0.5f, h/2+1.8333f, true);
	setTexflush();

	setFrame(bp0, bw);
	setTex0(bp1, hw);
	setRgba((int)(gain * 128.0f), (int)(gain * 128.0f), (int)(gain * 128.0f), 128);
	blit(0, 0, w, h, 0.25f, 0.25f, w/2 + 0.25f, h/2 + 0.25f, false, true, true);
	setTexflush();
	setRgba(128, 128, 128, 128);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::motionBlur(u32 strength)
{
	setTex0(m_renderer.backBuffer(), Fbw, 0, Tw, Th, 1, 0);
	setAlpha(0, 1, 2, 1, strength);
	blit(0, 0, 512, 448, true, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::fadeFrame(uint r, uint g, uint b, float strength)
{
	if (strength > 1.0)
		strength = 1.0f;

	if (strength < 0.0f) 
		strength = 0.0f;

	setRgba(r, g, b, 128);
	setAlpha(0, 2, 2, 1, u32(strength * 128.0f));
	blit(0, 0, 512, 448, true, false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectHelper::fadeDownFrame(uint r, uint g, uint b, float strength)
{
	if (strength > 1.0)
		strength = 1.0f;

	if (strength < 0.0f) 
		strength = 0.0f;

	setRgba(r, g, b, 128);
	setAlpha(1, 0, 2, 2, u32(strength * 128.0f));
	blit(0, 0, 512, 448, true, false);
}


	}
}
