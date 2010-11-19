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

#ifndef zenic_ps2_Vif_h
#define zenic_ps2_Vif_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

namespace vif
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum // vif codes
{
	Nop			= 0x00,
	Stcycl		= 0x01,
	Offset		= 0x02,
	Base		= 0x03,
	Itop		= 0x04,
	Stmod		= 0x05,
	Mskpath3	= 0x06,
	Mark		= 0x07,
	Flushe		= 0x10,
	Flush		= 0x11,
	Flusha		= 0x13,
	Mscal		= 0x14,
	Mscalf		= 0x15,
	Mscnt		= 0x17,
	Stmask		= 0x20,
	Strow		= 0x30,
	Stcol		= 0x31,
	Mpg			= 0x4a,
	Direct		= 0x50,
	DirectHl	= 0x51,
	Unpack		= 0x60
};

enum // Unpack formats
{
	S_32 = 0, S_16 = 1, S_8 = 2,
	V2_32 = 4, V2_16 = 5, V2_8 = 6,
	V3_32 = 8, V3_16 = 9, V3_8 = 10,
	V4_32 = 12, V4_16 = 13, V4_8 = 14,
	V4_5 = 15
};

#define VIF_NOP() (0)
#define VIF_STCYCL(wl,cl) VIF_CODE(zenic::ps2::vif::Stcycl, 0, ((wl)<<8)|(cl))
#define VIF_OFFSET(offset) VIF_CODE(zenic::ps2::vif::Offset, 0, (offset))
#define VIF_BASE(base) VIF_CODE(zenic::ps2::vif::Base, 0, (base))
#define VIF_ITOP(addr) VIF_CODE(zenic::ps2::vif::Itop, 0, (addr))
#define VIF_STMOD(mode) VIF_CODE(zenic::ps2::vif::Stmod, 0, (mode))
#define VIF_MSKPATH3(mask) VIF_CODE(zenic::ps2::vif::Mskpath3, 0, (mask))
#define VIF_MARK(mark) VIF_CODE(zenic::ps2::vif::Mark, 0, (mark))
#define VIF_FLUSHE() VIF_CODE(zenic::ps2::vif::Flushe, 0, 0)
#define VIF_FLUSH() VIF_CODE(zenic::ps2::vif::Flush, 0, 0)
#define VIF_FLUSHA() VIF_CODE(zenic::ps2::vif::Flusha, 0, 0)
#define VIF_MSCAL(execaddr) VIF_CODE(zenic::ps2::vif::Mscal, 0, (execaddr))
#define VIF_MSCNT() VIF_CODE(zenic::ps2::vif::Mscnt, 0, 0)
#define VIF_MSCALF(execaddr) VIF_CODE(zenic::ps2::vif::Mscalf, 0, (execaddr))
#define VIF_STMASK() VIF_CODE(zenic::ps2::vif::Stmask, 0, 0)
#define VIF_STROW() VIF_CODE(zenic::ps2::vif::Strow, 0, 0)
#define VIF_STCOL() VIF_CODE(zenic::ps2::vif::Stcol, 0, 0)
#define VIF_MPG(size,loadaddr) VIF_CODE(zenic::ps2::vif::Mpg, (size), (loadaddr))
#define VIF_DIRECT(size) VIF_CODE(zenic::ps2::vif::Direct, 0, (size))
#define VIF_DIRECTHL(size) VIF_CODE(zenic::ps2::vif::DirectHl, 0, (size))
#define VIF_UNPACK(type, quadCount, addr) VIF_CODE(zenic::ps2::vif::Unpack|(type), (quadCount), (addr))
#define VIF_UNPACKU(type, quadCount, addr) VIF_CODE(zenic::ps2::vif::Unpack|(type), (quadCount), (addr) | (1<<14))
#define VIF_UNPACKR(type, quadCount, addr) u32(VIF_UNPACK((type), (quadCount), (addr)) | (1<<15))
#define VIF_UNPACKRU(type, quadCount, addr) u32(VIF_UNPACK((type), (quadCount), (addr)) | (1<<15) | (1<<14))
#define VIF_UNPACKRUMSK(type, quadCount, addr) u32(VIF_UNPACK((type), (quadCount), (addr)) | (1<<15) | (1<<14) | (1<<28))

#define VIF_CODE(cmd,num,immediate) (((u32(cmd))<<24)|((u32(num))<<16)|(u32(immediate)))

enum // Decompression modes
{
	Normal = 0,
	OffsetDecompression, // would conflict with vif code
	Difference
};


enum // VIF STAT
{
	ProgramRunning	= 0x04,
	Unpacking		= 0x03,
	GifTransfer		= 0x08
};

// vif1 related registers

#define VIF1_STAT	((volatile u32 *)(0x10003c00))
#define VIF1_FBRST	((volatile u32 *)(0x10003c10))
#define VIF1_ERR	((volatile u32 *)(0x10003c20))
#define VIF1_MARK	((volatile u32 *)(0x10003c30))
#define VIF1_CYCLE	((volatile u32 *)(0x10003c40))
#define VIF1_MODE	((volatile u32 *)(0x10003c50))
#define VIF1_NUM	((volatile u32 *)(0x10003c60))
#define VIF1_MASK	((volatile u32 *)(0x10003c70))
#define VIF1_CODE	((volatile u32 *)(0x10003c80))
#define VIF1_ITOPS	((volatile u32 *)(0x10003c90))
#define VIF1_BASE	((volatile u32 *)(0x10003ca0))
#define VIF1_OFST	((volatile u32 *)(0x10003cb0))
#define VIF1_TOPS	((volatile u32 *)(0x10003cc0))
#define VIF1_ITOP	((volatile u32 *)(0x10003cd0))
#define VIF1_TOP	((volatile u32 *)(0x10003ce0))

typedef struct 
{
	unsigned VPS: 2;	// VIF1 pipeline status
	unsigned VEW: 1;	// VIF1 E-bit wait
	unsigned VGW: 1;	// VIF1 GIF wait
	unsigned p0 : 2;
	unsigned MRK: 1;	// VIF1 MARK detected flag
	unsigned DBF: 1;	// Duble Buffer Flag
	unsigned VSS: 1;	// VIF1 stop stall
	unsigned VFS: 1;	// VIF1 ForceBreak stall
	unsigned VIS: 1;	// VIF1 interrupt stall
	unsigned INT: 1;	// Interrupt bit detected flag
	unsigned ERO: 1;	// Mismatch Error detected flag
	unsigned ER1: 1;	// Reserved Instruction Error detected flag
	unsigned p1 : 9;
	unsigned FDR: 1;        // VIF1-FIFO direction
	unsigned FQC: 5;	// VIF1-FIFO valid data counter
	unsigned p2 : 3;

} sVIF1_STAT;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

}

}

#endif


