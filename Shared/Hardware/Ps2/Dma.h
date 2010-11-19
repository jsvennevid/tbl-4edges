///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_ps2_Dma_h
#define zenic_ps2_Dma_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Base/Io/Log.h>
#include "Vif.h"

#ifdef ZENIC_WIN32
#pragma warning(disable : 4311) // pointer truncation from
#pragma warning(disable : 4100) // unreferenced formal parameter
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define D0_CHCR         ((volatile u32 *)(0x10008000))
#define D0_MADR         ((volatile u32 *)(0x10008010))
#define D0_QWC          ((volatile u32 *)(0x10008020))
#define D0_TADR         ((volatile u32 *)(0x10008030))

#define D1_CHCR         ((volatile u32 *)(0x10009000))
#define D1_MADR         ((volatile u32 *)(0x10009010))
#define D1_QWC          ((volatile u32 *)(0x10009020))
#define D1_TADR         ((volatile u32 *)(0x10009030))

#define D2_CHCR         ((volatile u32 *)(0x1000a000))
#define D2_MADR         ((volatile u32 *)(0x1000a010))
#define D2_QWC          ((volatile u32 *)(0x1000a020))
#define D2_TADR         ((volatile u32 *)(0x1000a030))

#define D8_CHCR         ((volatile u32 *)(0x1000d000))
#define D8_MADR         ((volatile u32 *)(0x1000d010))
#define D8_QWC          ((volatile u32 *)(0x1000d020))
#define D8_SADR         ((volatile u32 *)(0x1000d080))

#define D9_CHCR         ((volatile u32 *)(0x1000d400))
#define D9_MADR         ((volatile u32 *)(0x1000d410))
#define D9_QWC          ((volatile u32 *)(0x1000d420))
#define D9_TADR         ((volatile u32 *)(0x1000d430))
#define D9_SADR         ((volatile u32 *)(0x1000d480))

#define D_STAT			((volatile u32 *)(0x1000e010))
#define D_CTRL			((volatile u32 *)(0x1000e000))

#define REF_TAG(addr, count)	((((u64)(addr)) << 32) | (0x3 << 28) | count)
#define CNT_TAG(count)			((((u64)(0x1)) << 28) | count)
#define END_TAG(count)			((((u64)(0x7)) << 28) | count)
#define NEXT_TAG(addr, count)	((((u64)(addr)) << 32) | (0x2 << 28) | count)
#define REFS_TAG(addr, count)	((((u64)(addr)) << 32) | (0x4 << 28) | count)
#define REFE_TAG(addr, count)	((((u64)(addr)) << 32) | (0x0 << 28) | count)
#define CALL_TAG(addr, count)	((((u64)(addr)) << 32) | (0x5 << 28) | count)
#define RET_TAG(count)					((((u64)(0x6)) << 28) | count)

#define CHCR(DIR,MOD,ASP,TTE,TIE,STR) ( (((u32)DIR)<<0) | (((u32)MOD)<<2) | \
                                        (((u32)ASP)<<4) | (((u32)TTE)<<6) | \
                                        (((u32)TIE)<<7) | (((u32)STR)<<8) )

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZENIC_FINAL
#define DMA_SANITY_CHECK() \
		ZENIC_ASSERT_DESC(calcSize() < m_size, "Trying to write outside dmabuffer size!");
#else
#define DMA_SANITY_CHECK()
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

namespace ps2
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Dma
{
public:

	Dma();
	Dma(void* data,u32 size);

	void addSrcRefTag(void* data, u32 size, u64 tteCode = 0);
	void addSrcRefTag(void* data, u32 size, u32 tteLower, u32 tteUpper);
	void addSrcCntTag(u64 tteCode = 0);
	void addSrcCntTag(u32 tteLower, u32 tteUpper);
	void addSrcEndTag(u64 tteCode = 0);
	void addSrcEndTag(u32 tteLower, u32 tteUpper);
	void addSrcNextTag(void* nextAddress, u64 tteCode = 0);
	void addSrcNextTag(void* data, u32 tteLower, u32 tteUpper);
	void addSrcRefsTag(void* data, u32 size, u64 tteCode = 0);
	void addSrcRefsTag(void* data, u32 size, u32 tteLower, u32 tteUpper);
	void addSrcRefeTag(void* data, u32 size, u64 tteCode = 0);
	void addSrcRefeTag(void* data, u32 size, u32 tteLower, u32 tteUpper);
	void addSrcCallTag(const void* data, u64 tteCode = 0);
	void addSrcCallTag(const void* data, u32 tteLower, u32 tteUpper);
	void addSrcRetTag(u64 tteCode = 0);
	void addSrcRetTag(u32 tteLower, u32 tteUpper);
	void addUnpack(int format, int addr, int num, int usetops = 0,int nosign = 1, int masking = 0);
	void endPacket();

	void stmod(const int mode) { add32((0x5<<24) | (mode&0x3)); }
	void stmask(const unsigned int mask) { add32(0x20<<24); add32(mask); }
	void strow()	{ add32(0x30<<24); }
	void stcol()	{ add32(0x31<<24); }
	void setMark(const s16& value) { add32(VIF_CODE(vif::Mark, 0, value)); }
	void addOffset(int offset) { add32((0x2 << 24) + offset); }
	void addFlushe() { add32(0x10 << 24); }
	void addFlusha() { add32(0x13 << 24); }
	void addBase(int num) { add32((3 << 24) + num); }
	void addMscalf(int addr) { add32((0x15 << 24) + addr); }
	void addMscnt() { add32(0x17 << 24); }
	void addMscal(int addr) { add32((0x14 << 24) + addr); }
	void addItop(int num) { add32((4 << 24) + num); }
	void addStcycl(int cl, int wl) { add32((1 << 24) + cl + (wl << 8)); }

	void* reserve(uint size);
	void* reserveEnd(uint size);

	void addDstCntTag(void* data,u64 tteCode = 0);
	void addDstCntsTag(void* data,u64 tteCode = 0);
	void addDstEndTag(void* data,u64 tteCode = 0);

	void add128(u64 value1,u64 value2);
	void add64(u64 value);
	void add32(u32 value);
	void addFloat(f32 value);
	void add16(u16 value);
	void add8(u8 value);

	void align(uint align);
	void align32(int align, int offset = 0);
		
	void setData(void* data,u32 size); 
	void* getData();
	void saveChain(const char* fileName);
	u32 calcSize() const;

	void* tagLocation() const;
	void* currentLocation() const;

	void beginDirect();
	void endDirect();

	static void ctrlStatus();
	static void stats();

	void sendChainVif0();
	void sendNormalVif0();
	static void waitVif0();

	void sendChainVif1();
	void sendNormalVif1();
	static void waitVif1();
	static void vif1Stats();

	void sendChainGif();
	void sendNormalGif();
	static void waitGif();

	void sendChainFromSp();
	void sendNormalFromSp();
	static void waitFromSp();

	void sendChainToSp();
	void sendNormalToSp();
	static void waitToSp();

	static void sendChainVif0(const void* data);
	static void sendNormalVif0(const void* data,u32 size);
	static void sendChainVif1(const void* data);
	static void sendNormalVif1(const void* data,u32 size);
	static void sendChainGif(const void* data);
	static void sendNormalGif(const void* data,u32 size);
	static void sendChainFromSp(const void* data);
	static void sendNormalFromSp(void* dest, const void* src, u32 size);
	static void sendChainToSp(const void* data);
	static void sendNormalToSp(const void* data,u32 size);

private:

	static u32 address(const void* mem);
	static void channelStat(uint id, u32 Chcr);

	void* m_data;

	void* m_tagLocation;
	void* m_currentLocation;
	u64* m_directRecord;
	u32 m_size;

	static void* m_saveCurrent; 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Dma.inl"

	}
}

#ifdef ZENIC_WIN32
#pragma warning(default : 4311) // pointer truncation from
#pragma warning(default : 4100) // unreferenced formal parameter
#endif


#endif //zenic_ps2_hardware_Dma_h


