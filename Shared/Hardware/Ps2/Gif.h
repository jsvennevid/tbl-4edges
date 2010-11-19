#ifndef zenic_ps2_Gif_h
#define zenic_ps2_Gif_h

namespace zenic
{

namespace ps2
{

namespace gif
{

#define GIF_MODE  ((volatile u32 *)(0x10003010))

// masking could be removed, but we keep it for now for safety precautions
#define GIF_BIT_ENCODE(val,ofs,count) (((u64)(val))&(((u64)~0)>>(64-count)))<<ofs

#define	GIFTAG(nloop,eop,pre,prim,flg,nreg) \
	(GIF_BIT_ENCODE((nloop),0,15)|\
	GIF_BIT_ENCODE((eop),15,1)|\
	GIF_BIT_ENCODE((pre),46,1)|\
	GIF_BIT_ENCODE((prim),47,11)|\
	GIF_BIT_ENCODE((flg),58,2)|\
	GIF_BIT_ENCODE((nreg),60,4))

enum
{
	Packed = 0,
	RegList,
	Image,
	Disable
};

enum { Eop = 1 };
enum { Pre = 1 };

enum
{
	PRIM	= 0,
	RGBAQ	= 1,
	ST		= 2,
	UV		= 3,
	XYZF2	= 4,
	XYZ2	= 5,
	TEX0_1	= 6,
	TEX0_2	= 7,
	CLAMP_1	= 8,
	CLAMP_2 = 9,
	FOG		= 10,
	XYZF3	= 12,
	XYZ3	= 13,
	A_D		= 14,
	NOP		= 15
};

}

}

}

#endif
