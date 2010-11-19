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

#ifndef zenic_ps2_hardware_Gs_h
#define zenic_ps2_hardware_Gs_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// forwards
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

namespace ps2
{

namespace gs
{

inline u32 encodeFloat(f32 f)
{
	return *(reinterpret_cast<u32*>(&f));
}

enum Psm
{
	CT32	= 0x00,		// 000000
	CT24	= 0x01,		// 000001
	CT16	= 0x02,		// 000010
	T8		= 0x13,		// 010011
	T4		= 0x14,		// 010100
	T8H		= 0x1b,		// 011011
	T4HL	= 0x24,		// 100100
	T4HH	= 0x2c,		// 101100
	Z32		= 0x30,		// 110000
	Z24		= 0x31,		// 110001
	Z16		= 0x32,		// 110010
	Z16S	= 0x3a		// 111010
};

enum XPixelSize
{
	XPIXELSIZE_320,
	XPIXELSIZE_512,
	XPIXELSIZE_640
};

enum YPixelSize
{
	YPIXELSIZE_1,
	YPIXELSIZE_2
};

// masking could be removed, but we keep it for now for safety precautions
#define GS_BIT_ENCODE(val,ofs,count) (((u64)(val))&(((u64)~0)>>(64-count)))<<ofs

// NOTE: this is actually a function, but we think of it as a macro
// not inlined here because of crappy gcc

#define GS_PAGE_ADDRESS(a) ((a)>>13)
#define GS_TEXTURE_ADDRESS(a) ((a)>>8)

// *** GS: GENERAL PURPOSE REGISTERS ***

#define GS_REG(r) (zenic::ps2::gs::r)

enum
{
	ALPHA_1			= 0x42,
	ALPHA_2			= 0x43,
	BITBLTBUF		= 0x50,
	CLAMP_1			= 0x08,
	CLAMP_2			= 0x09,
	COLCLAMP		= 0x46,
	DIMX			= 0x44,
	DTHE			= 0x45,
	FBA_1			= 0x4a,
	FBA_2			= 0x4b,
	FINISH			= 0x61,
	FOG				= 0x0a,
	FOGCOL			= 0x3d,
	FRAME_1			= 0x4c,
	FRAME_2			= 0x4d,
	HWREG			= 0x54,
	LABEL			= 0x62,
	MIPTBP1_1		= 0x34,
	MIPTBP1_2		= 0x35,
	MIPTBP2_1		= 0x36,
	MIPTBP2_2		= 0x37,
	NOP				= 0x7f,
	PABE			= 0x49,
	PRIM			= 0x00,
	PRMODE			= 0x1b,
	PRMODECONT		= 0x1a,
	RGBAQ			= 0x01,
	SCANMSK			= 0x22,
	SCISSOR_1		= 0x40,
	SCISSOR_2		= 0x41,
	SIGNAL			= 0x60,
	ST				= 0x02,
	TEST_1			= 0x47,
	TEST_2			= 0x48,
	TEX0_1			= 0x06,
	TEX0_2			= 0x07,
	TEX1_1			= 0x14,
	TEX1_2			= 0x15,
	TEX2_1			= 0x16,
	TEX2_2			= 0x17,
	TEXA			= 0x3b,
	TEXCLUT			= 0x1c,
	TEXFLUSH		= 0x3f,
	TRXDIR			= 0x53,
	TRXPOS			= 0x51,
	TRXREG			= 0x52,
	UV				= 0x03,
	XYOFFSET_1		= 0x18,
	XYOFFSET_2		= 0x19,
	XYZ2			= 0x05,
	XYZ3			= 0x0d,
	XYZF2			= 0x04,
	XYZF3			= 0x0c,
	ZBUF_1			= 0x4e,
	ZBUF_2			= 0x4f
};

enum Primitive
{
	Point,
	Line,
	Linestrip,
	Triangle,
	Tristrip,
	Trifan,
	Sprite
};

// shading method
enum Shading
{
	Flat,
	Gouraud
};

// texture mapping
enum { Texturing = 1 };

// fogging
enum { Fog = 1 };

// alpha blending
enum { Blend = 1 };

// 1-pass antialiasing
enum { Antialias = 1 };

// texture coordinates
enum
{
	Stq,
	Uv
};

// context
enum Context
{
	Context1,
	Context2
};

// alpha test method

namespace alpha
{

// alpha test enable
enum { On = 1 };

enum
{
	Never,
	Always,
	Less,
	Equal,
	GreaterEqual,
	Greater,
	NotEqual
};

enum
{
	Keep,
	FrameBuffer,
	ZBuffer,
	RGB
};

}

namespace zbuffer
{

enum
{
	Never,
	Always,
	GreaterEqual,
	Greater
};

}

enum { ColorClamp = 1 };

enum { Dithering = 1 };

/*
// signal event
#define GS_CSR_SIGOFF		0
#define GS_CSR_SIGON		1

// finish event
#define	GS_CSR_FINOFF		0
#define	GS_CSR_FINON		1

// horizontal sync event
#define	GS_CSR_HSOFF		0
#define	GS_CSR_HSON		1

// vertical sync event
#define	GS_CSR_VSOFF		0
#define	GS_CSR_VSON		1

// rectangular area write termination
#define	GS_CSR_RAWTOFF		0
#define	GS_CSR_RAWTON		1

// drawing suspend / fifo clear
#define	GS_CSR_FLUSHOFF		0
#define	GS_CSR_FLUSHON		1

// gs system reset
#define	GS_CSR_RSTOFF		0
#define	GS_CSR_RSTON		1

// field currently displayed
#define	GS_CSR_FEVEN		0
#define	GS_CSR_FODD		1

// fifo status
#define	GS_CSR_FIFO_RUNNING	0	// neither empty or almost full
#define	GS_CSR_FIFO_EMPTY	1	// empty
#define	GS_CSR_FIFO_AFULL	2	// almost full
*/

#define GS_ALPHA(a,b,c,d,fix) \
	(GS_BIT_ENCODE((a),0,2)|\
	GS_BIT_ENCODE((b),2,2)|\
	GS_BIT_ENCODE((c),4,2)|\
	GS_BIT_ENCODE((d),6,2)|\
	GS_BIT_ENCODE((fix),32,8))

#define GS_BITBLTBUF(sbp,sbw,spsm,dbp,dbw,dpsm) \
	(GS_BIT_ENCODE((sbp),0,14)|\
	GS_BIT_ENCODE((sbw),16,6)|\
	GS_BIT_ENCODE((spsm),24,6)|\
	GS_BIT_ENCODE((dbp),32,14)|\
	GS_BIT_ENCODE((dbw),48,6)|\
	GS_BIT_ENCODE((dpsm),56,6))

#define GS_CLAMP(wms,wmt,minu,maxu,minv,maxv) \
	(GS_BIT_ENCODE((wms),0,2)|\
	GS_BIT_ENCODE((wmt),2,2)|\
	GS_BIT_ENCODE((minu),4,10)|\
	GS_BIT_ENCODE((maxu),14,10)|\
	GS_BIT_ENCODE((minv),24,10)|\
	GS_BIT_ENCODE((maxv),34,10))

#define GS_COLCLAMP(clamp) GS_BIT_ENCODE((clamp),0,1)

#define GS_DIMX(dm00,dm01,dm02,dm03,dm10,dm11,dm12,dm13,dm20,dm21,dm22,dm23,dm30,dm31,dm32,dm33) \
	(GS_BIT_ENCODE((dm00),0,3)|\
	GS_BIT_ENCODE((dm01),4,3)|\
	GS_BIT_ENCODE((dm02),8,3)|\
	GS_BIT_ENCODE((dm03),12,3)|\
	GS_BIT_ENCODE((dm10),16,3)|\
	GS_BIT_ENCODE((dm11),20,3)|\
	GS_BIT_ENCODE((dm12),24,3)|\
	GS_BIT_ENCODE((dm13),28,3)|\
	GS_BIT_ENCODE((dm20),32,3)|\
	GS_BIT_ENCODE((dm21),36,3)|\
	GS_BIT_ENCODE((dm22),40,3)|\
	GS_BIT_ENCODE((dm23),44,3)|\
	GS_BIT_ENCODE((dm30),48,3)|\
	GS_BIT_ENCODE((dm31),52,3)|\
	GS_BIT_ENCODE((dm32),56,3)|\
	GS_BIT_ENCODE((dm33),60,3))

#define GS_DTHE(dthe) GS_BIT_ENCODE((dthe),0,1)

#define GS_FBA(fba) GS_BIT_ENCODE((fba),0,1)

#define GS_FINISH() (0)

#define GS_FOG(f) GS_BIT_ENCODE((f),56,8)

#define GS_FOGCOL(fcr,fcg,fcb) \
	(GS_BIT_ENCODE((fcr),0,8)|\
	GS_BIT_ENCODE((fcg),8,8)|\
	GS_BIT_ENCODE((fcb),16,8))

#define GS_FRAME(fbp,fbw,psm,fbmsk) \
	(GS_BIT_ENCODE((fbp),0,9)|\
	GS_BIT_ENCODE((fbw),16,6)|\
	GS_BIT_ENCODE((psm),24,6)|\
	GS_BIT_ENCODE((fbmsk),32,32))

#define GS_HWREG(data) ((md_u64_t)data)

#define GS_LABEL(id,idmsk) \
	(GS_BIT_ENCODE((id),0,32)|\
	GS_BIT_ENCODE((idmsk),32,32))

#define GS_MIPTBP1_ENCODE(tbp1,tbw1,tbp2,tbw2,tbp3,tbw3) \
	(GS_BIT_ENCODE((tbp1),0,14)|\
	GS_BIT_ENCODE((tbw1),14,6)|\
	GS_BIT_ENCODE((tbp2),20,14)|\
	GS_BIT_ENCODE((tbw2),34,6)|\
	GS_BIT_ENCODE((tbp3),40,14)|\
	GS_BIT_ENCODE((tbw3),54,6))

#define GS_MIPTBP2_ENCODE(tbp4,tbw4,tbp5,tbw5,tbp6,tbw6) \
	(GS_BIT_ENCODE((tbp4),0,14)|\
	GS_BIT_ENCODE((tbw4),14,6)|\
	GS_BIT_ENCODE((tbp5),20,14)|\
	GS_BIT_ENCODE((tbw5),34,6)|\
	GS_BIT_ENCODE((tbp6),40,14)|\
	GS_BIT_ENCODE((tbw6),54,6))

#define GS_PABE(pabe) GS_BIT_ENCODE((pabe),0,1)

#define GS_PRIM(prim,iip,tme,fge,abe,aa1,fst,ctxt,fix) \
	(GS_BIT_ENCODE((prim),0,3)|\
	GS_BIT_ENCODE((iip),3,1)|\
	GS_BIT_ENCODE((tme),4,1)|\
	GS_BIT_ENCODE((fge),5,1)|\
	GS_BIT_ENCODE((abe),6,1)|\
	GS_BIT_ENCODE((aa1),6,1)|\
	GS_BIT_ENCODE((fst),8,1)|\
	GS_BIT_ENCODE((ctxt),9,1)|\
	GS_BIT_ENCODE((fix),10,1))

#define GS_CLAMP(wms,wmt,minu,maxu,minv,maxv) \
	(GS_BIT_ENCODE((wms),0,2)|\
	GS_BIT_ENCODE((wmt),2,2)|\
	GS_BIT_ENCODE((minu),4,10)|\
	GS_BIT_ENCODE((maxu),14,10)|\
	GS_BIT_ENCODE((minv),24,10)|\
	GS_BIT_ENCODE((maxv),34,10))

#define GS_PRMODE(iip,tme,fge,abe,aa1,fst,ctxt,fix) \
	(GS_BIT_ENCODE((iip),3,1)|\
	GS_BIT_ENCODE((tme),4,1)|\
	GS_BIT_ENCODE((fge),5,1)|\
	GS_BIT_ENCODE((abe),6,1)|\
	GS_BIT_ENCODE((aa1),6,1)|\
	GS_BIT_ENCODE((fst),8,1)|\
	GS_BIT_ENCODE((ctxt),9,1)|\
	GS_BIT_ENCODE((fix),10,1))


#define GS_PRMODECONT(ac) GS_BIT_ENCODE((ac),0,1)

#define GS_RGBAQ(r,g,b,a,q) \
	(GS_BIT_ENCODE((r),0,8)|\
	GS_BIT_ENCODE((g),8,8)|\
	GS_BIT_ENCODE((b),16,8)|\
	GS_BIT_ENCODE((a),24,8)|\
	GS_BIT_ENCODE(zenic::ps2::gs::encodeFloat(q),32,32))

#define GS_SCANMSK(msk) GS_BIT_ENCODE((msk),0,2)

#define GS_SCISSOR(scax0,scax1,scay0,scay1) \
	(GS_BIT_ENCODE(scax0,0,11)|\
	GS_BIT_ENCODE(scax1,16,11)|\
	GS_BIT_ENCODE(scay0,32,11)|\
	GS_BIT_ENCODE(scay1,48,11))

#define GS_SIGNAL(id,idmsk) \
	(GS_BIT_ENCODE((id),0,32)|\
	GS_BIT_ENCODE((idmsk),32,32))

#define GS_ST(s,t) \
	(GS_BIT_ENCODE(zenic::ps2::gs::encodeFloat((s)),0,32)|\
	GS_BIT_ENCODE(zenic::ps2::gs::encodeFloat((t)),32,32))

#define GS_TEST(ate,atst,aref,afail,date,datm,zte,ztst) \
	(GS_BIT_ENCODE((ate),0,1)|\
	GS_BIT_ENCODE((atst),1,3)|\
	GS_BIT_ENCODE((aref),4,8)|\
	GS_BIT_ENCODE((afail),12,2)|\
	GS_BIT_ENCODE((date),14,1)|\
	GS_BIT_ENCODE((datm),15,1)|\
	GS_BIT_ENCODE((zte),16,1)|\
	GS_BIT_ENCODE((ztst),17,2))

#define GS_TEX0(tbp0,tbw,psm,tw,th,tcc,tfx,cbp,cpsm,csm,csa,cld) \
	(GS_BIT_ENCODE((tbp0),0,14)|\
	GS_BIT_ENCODE((tbw),14,6)|\
	GS_BIT_ENCODE((psm),20,6)|\
	GS_BIT_ENCODE((tw),26,4)|\
	GS_BIT_ENCODE((th),30,4)|\
	GS_BIT_ENCODE((tcc),34,1)|\
	GS_BIT_ENCODE((tfx),35,2)|\
	GS_BIT_ENCODE((cbp),37,14)|\
	GS_BIT_ENCODE((cpsm),51,4)|\
	GS_BIT_ENCODE((csm),55,1)|\
	GS_BIT_ENCODE((csa),56,5)|\
	GS_BIT_ENCODE((cld),61,3))

#define GS_TEX1(lcm,mxl,mmag,mmin,mtba,l,k) \
	(GS_BIT_ENCODE((lcm),0,1)|\
	GS_BIT_ENCODE((mxl),2,3)|\
	GS_BIT_ENCODE((mmag),5,1)|\
	GS_BIT_ENCODE((mmin),6,3)|\
	GS_BIT_ENCODE((mtba),9,1)|\
	GS_BIT_ENCODE((l),19,2)|\
	GS_BIT_ENCODE((k),32,12))

#define	GS_TEX2(psm,cbp,cpsm,csm,csa,cld) \
	(GS_BIT_ENCODE((psm),20,6)|\
	GS_BIT_ENCODE((cbp),37,14)|\
	GS_BIT_ENCODE((cpsm),51,4)|\
	GS_BIT_ENCODE((csm),55,1)|\
	GS_BIT_ENCODE((csa),56,5)|\
	GS_BIT_ENCODE((cld),61,3))

#define GS_TEXA(ta0,aem,ta1) \
	(GS_BIT_ENCODE((ta0),0,8)|\
	GS_BIT_ENCODE((aem),15,1)|\
	GS_BIT_ENCODE((ta1),32,8))

#define GS_TEXCLUT(cbw,cou,cov) \
	(GS_BIT_ENCODE((cbw),0,6)|\
	GS_BIT_ENCODE((cou),6,6)|\
	GS_BIT_ENCODE((cov),12,10))

#define GS_TEXFLUSH() (0)

#define GS_TRXDIR(xdir) GS_BIT_ENCODE((xdir),0,2)

#define GS_TRXPOS(ssax,ssay,dsax,dsay,dir) \
	(GS_BIT_ENCODE((ssax),0,11)|\
	GS_BIT_ENCODE((ssay),16,11)|\
	GS_BIT_ENCODE((dsax),32,11)|\
	GS_BIT_ENCODE((dsay),48,11)|\
	GS_BIT_ENCODE((dir),59,2))

#define	GS_TRXREG(rrw,rrh) \
	(GS_BIT_ENCODE((rrw),0,12)|\
	GS_BIT_ENCODE((rrh),32,12))

#define GS_UV(u,v) \
	(GS_BIT_ENCODE((u),0,14)|\
	GS_BIT_ENCODE((v),16,14))

#define GS_XYOFFSET(ofx,ofy) \
	(GS_BIT_ENCODE((ofx),0,16)|\
	GS_BIT_ENCODE((ofy),32,16))

#define GS_XYZ2(x,y,z) \
	(GS_BIT_ENCODE((x),0,16)|\
	GS_BIT_ENCODE((y),16,16)|\
	GS_BIT_ENCODE((z),32,32))

#define	GS_XYZ3(x,y,z) GS_XYZ2_ENCODE((x),(y),(z))

#define	GS_XYZF2(x,y,z,f) \
	(GS_BIT_ENCODE((x),0,16)|\
	GS_BIT_ENCODE((y),16,16)|\
	GS_BIT_ENCODE((z),32,24)|\
	GS_BIT_ENCODE((f),56,8))

#define GS_XYZF3(x,y,z,f) GS_XYZF2_ENCODE((x),(y),(z),(f))

#define	GS_ZBUF(zbp,psm,zmsk) \
	(GS_BIT_ENCODE((zbp),0,9)|\
	GS_BIT_ENCODE((psm),24,4)|\
	GS_BIT_ENCODE((zmsk),32,1))

// *** PRIVILEGED REGISTERS ***

#define	GS_PREG(r) (GS_PREG_##r)

#define GS_PREG_PMODE		0x0000
#define GS_PREG_SMODE_1		0x0010
#define GS_PREG_SMODE_2		0x0020
#define GS_PREG_SRFSH		0x0030
#define GS_PREG_SYNCH_1		0x0040
#define GS_PREG_SYNCH_2		0x0060
#define GS_PREG_DISPFB_1	0x0070
#define GS_PREG_DISPLAY_1	0x0080
#define GS_PREG_DISPFB_2	0x0090
#define GS_PREG_DISPLAY_2	0x00a0
#define GS_PREG_EXTBUF		0x00b0
#define GS_PREG_EXTDATA		0x00c0
#define GS_PREG_EXTWRITE	0x00d0
#define GS_PREG_BGCOLOR		0x00e0
#define GS_PREG_CSR		0x1000
#define GS_PREG_IMR		0x1010
#define	GS_PREG_BUSDIR		0x1040
#define	GS_PREG_SIGBLID		0x1080

#define GS_WRITE(r,v)		*((volatile u64*)(0x12000000+GS_PREG(r))) = (v)
#define	GS_READ(r)		*((volatile u64*)(0x12000000+GS_PREG(r)))

#define GS_PMODE(en1,en2,mmod,amod,slbg,alp) \
	(GS_BIT_ENCODE((en1),0,1)|\
	GS_BIT_ENCODE((en2),1,1)|\
	GS_BIT_ENCODE(1,2,3)|\
	GS_BIT_ENCODE((mmod),5,1)|\
	GS_BIT_ENCODE((amod),6,1)|\
	GS_BIT_ENCODE((slbg),7,1)|\
	GS_BIT_ENCODE((alp),8,8))

#define GS_DISPFB(fbp,fbw,psm,dbx,dby) \
	(GS_BIT_ENCODE((fbp),0,9)|\
	GS_BIT_ENCODE((fbw),9,6)|\
	GS_BIT_ENCODE((psm),15,5)|\
	GS_BIT_ENCODE((dbx),32,11)|\
	GS_BIT_ENCODE((dby),43,11))

#define GS_BGCOLOR(r,g,b) \
	(GS_BIT_ENCODE((r),0,8)|\
	GS_BIT_ENCODE((g),8,8)|\
	GS_BIT_ENCODE((b),16,8))

#define GS_BUSDIR(dir)	GS_BIT_ENCODE((dir),0,1)

#define GS_DISPLAY(dx,dy,magh,magv,dw,dh) \
	(GS_BIT_ENCODE((dx),0,12)|\
	GS_BIT_ENCODE((dy),12,11)|\
	GS_BIT_ENCODE((magh),23,4)|\
	GS_BIT_ENCODE((magv),27,2)|\
	GS_BIT_ENCODE((dw),32,12)|\
	GS_BIT_ENCODE((dh),44,11))

#define GS_EXTBUF(exbp,xbw,fbin,wffmd,emoda,emodc,wdx,wdy) \
	(GS_BIT_ENCODE((exbp),0,14)|\
	GS_BIT_ENCODE((exbw),14,6)|\
	GS_BIT_ENCODE((fbin),20,2)|\
	GS_BIT_ENCODE((wffmd),22,1)|\
	GS_BIT_ENCODE((emoda),23,2)|\
	GS_BIT_ENCODE((emodc),25,2)|\
	GS_BIT_ENCODE((wdx),32,11)|\
	GS_BIT_ENCODE((wdy),43,11))

#define GS_EXTDATA(sx,sy,smph,smpv,ww,wh) \
	(GS_BIT_ENCODE((sx),0,12)|\
	GS_BIT_ENCODE((sy),12,11)|\
	GS_BIT_ENCODE((smph),23,4)|\
	GS_BIT_ENCODE((smpv),27,2)|\
	GS_BIT_ENCODE((ww),32,12)|\
	GS_BIT_ENCODE((wh),44,11))

#define GS_EXTWRITE(write) GS_BIT_ENCODE((write),0,1)

#define	GS_CSR(signal,finish,hsint,vsint,edwint,flush,reset,nfield,field,fifo,rev,id) \
	(GS_BIT_ENCODE((signal),0,1)|\
	GS_BIT_ENCODE((finish),1,1)|\
	GS_BIT_ENCODE((hsint),2,1)|\
	GS_BIT_ENCODE((vsint),3,1)|\
	GS_BIT_ENCODE((edwint),4,1)|\
	GS_BIT_ENCODE((flush),8,1)|\
	GS_BIT_ENCODE((reset),9,1)|\
	GS_BIT_ENCODE((nfield),12,1)|\
	GS_BIT_ENCODE((field),13,1)|\
	GS_BIT_ENCODE((fifo),16,8)|\
	GS_BIT_ENCODE((rev),24,8)|\
	GS_BIT_ENCODE((id),0,1))

#define GS_SMODE2(it,ffmd,dpms) \
	(GS_BIT_ENCODE((it),0,1)|\
	GS_BIT_ENCODE((ffmd),1,1)|\
	GS_BIT_ENCODE((dpms),2,2))

}

}

}

#endif //zenic_ps2_hardware_Gs_h


