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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PostEffectBuilder.h"
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Vif.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>

extern "C" u32 VUIndexPass_Begin __attribute__((section(".vudata")));
extern "C" u32 VUIndexPass_End __attribute__((section(".vudata")));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PostEffectBuilder::buildIndexChain(u8* dmaData, const u32 uVRamSrc, const u32 uVRamDst, const u32 uVRamZbufArg, 
										const u32 uVRamPaletteLocation, const s32 x, const s32 y, 
										const s32 dimx, const s32 dimy, const bool bEnableZwrite, 
										const u32 uBufferWidth)
{
	ZENIC_ASSERT(dmaData);

	Dma dmaChain(dmaData, 64 * 1024);
	dmaChain.addSrcRetTag();

	// record location
    
	// snap to surrounding page grid
	const s32 X0 = x & (~63);
	const s32 Y0 = y & (~31);
	const s32 dimX = (dimx+(x&63)+63) & (~63);
	const s32 dimY = (dimy+(y&31)+31) & (~31);
	ZENIC_ASSERT((X0&63)==0 && (dimX&63)==0);
	ZENIC_ASSERT((Y0&31)==0 && (dimY&31)==0);
	s32 num_pages = (dimX/64)*(dimY/32);

	// define horizontal area in pages
	ZENIC_ASSERT((uBufferWidth&63)==0 && dimX<=(s32) uBufferWidth);
	const u32 uActHorzPages = dimX/64;
	const u32 uRemHorzPages = (uBufferWidth/64) - uActHorzPages;

	// Make initial locations in vram
	const u32 uVRamOffs = (Y0/32)*(uBufferWidth/64) + (X0/64);
	u32 uVRamFrame = uVRamDst + uVRamOffs;
	u32 uVRamTex0 = (uVRamSrc + uVRamOffs)*32;
	u32 uVRamZBuf = uVRamZbufArg + uVRamOffs;

	//const u32 uNumPageLines = dimY/32;

	// deliver attributes
	u32 uCurPageCount = 0;
	bool bFirstRun = true;

	while (num_pages > 0)
	{
		const s32 deliver_pages = (num_pages > 40) ? 40 : num_pages;
		dmaChain.addItop(deliver_pages);        // num_pages

		// set Mode to difference mode
		dmaChain.stmod(2);
		dmaChain.addStcycl(3,1);

		// set FRAME and MASK
		// set row FRAME_2(uVRamFrame, 1, 0, uMask)
		dmaChain.stmask(0x58585858);
		dmaChain.strow();

		////dmaChain.add64( PS2_GS_SETREG_FRAME(uVRamFrame, 1, 0, 0) );
		////dmaChain.add64( PS2_GS_FRAME_2 );
		dmaChain.add64(GS_FRAME(uVRamFrame, 1, 0, 0));
		dmaChain.add64(GS_REG(FRAME_2));

		// unfold the next "deliver_pages" FRAMEs
		unfoldAddList(dmaChain, &uVRamFrame, bFirstRun, uCurPageCount, deliver_pages, uActHorzPages, 1, (uRemHorzPages+1), 3);

		// set TEX0
		// set row TEX0_2(uVRamTex0, 2*uWidth/64, ..)
		dmaChain.stmask(0x54545454);
		dmaChain.strow();

		////dmaChain.add64( PS2_GS_SETREG_TEX0(uVRamTex0, 2, 0x13, 7, 6, 1, 0, uVRamPaletteLocation*32, 0, 0, 0, 1) );
		////dmaChain.add64( PS2_GS_TEX0_2 );
		dmaChain.add64(GS_TEX0(uVRamTex0, 2, 0x13, 7, 6, 1, 0, uVRamPaletteLocation*32, 0, 0, 0, 1));
		dmaChain.add64(GS_REG(TEX0_2));

		// unfold the next "deliver_pages" TEX0s
		unfoldAddList(dmaChain, &uVRamTex0, bFirstRun, uCurPageCount, deliver_pages, uActHorzPages, 32, (uRemHorzPages+1)*32, 4);

		// set ZBUF
		// set row ZBUF_2(uVRamZBuf, 1, 1)
		dmaChain.strow();

		////dmaChain.add64( PS2_GS_SETREG_ZBUF(uVRamZBuf, 1, ((bEnableZwrite)?0:1)) );
		////dmaChain.add64( PS2_GS_ZBUF_2 );
		dmaChain.add64(GS_ZBUF(uVRamZBuf, 1, ((bEnableZwrite)?0:1)));
		dmaChain.add64(GS_REG(ZBUF_2));

		// unfold the next "deliver_pages" ZBUFs
		uCurPageCount = unfoldAddList(dmaChain, &uVRamZBuf, bFirstRun, uCurPageCount, deliver_pages, uActHorzPages, 1, 
									  (uRemHorzPages+1), 5);

		// disable difference mode and restore stcycl pattern
		dmaChain.stmod(0);
		dmaChain.addStcycl(1,1);

		// mscall
		dmaChain.addMscal(2);
		//dmaChain.add32(0);

		// next
		num_pages -= deliver_pages;
		bFirstRun = false;

		if(num_pages > 0)
		{
			dmaChain.addUnpack(vif::S_32, 1, 1, true);

			// add information (-1) means same settings as for last batch
			dmaChain.add32( 0xffffffff );   // Flag
		}
	}

	// terminate and return physical location of chain
	//dmaChain.ret();

	dmaChain.align(16);
	dmaChain.endPacket();
}

#define SCE_GS_SET_CLAMP(wms, wmt, minu, maxu, minv, maxv) \
	((u64)(wms)         | ((u64)(wmt) << 2) | \
	((u64)(minu) << 4)  | ((u64)(maxu) << 14) | \
	((u64)(minv) << 24) | ((u64)(maxv) << 34))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// my vu program


void PostEffectBuilder::uploadIndexChain(Dma& dmaChain, const void * pPhysCallChain, const int channel_number, 
										 const u32 uMask,const bool bIsZbuffer, const u32 uZdepth, 
										 const bool bAlphaEnabled, const void * pPhysPaletteUploadChain)
{
	// make bools
	const bool bRedOrBlue = (channel_number&1)==0;
	const bool bRedOrGreen = channel_number<2;
    
	// if VU code was never created
	//if(g_pVUcode==NULL)
	//   g_pVUcode = vumanager.createVuCode(vu1rgba_indexer_vu, 512);
    
	// control
	static bool bOldRedOrGreen = false;
	static u32 uOldMask = 0;
	static int old_channel_number = 0;
	static bool bOldIsZbuffer = false;
	static u32 uOldZdepth = 0;
	static bool bOldAlphaEnabled = false;
    
	// if code hasn't been uploaded already.
	//if (ps2::hardware::Vu1::getProgram(ps2::hardware::Vu1::RgbaIndexer) != -1)
	{
		// set default to change
		bOldRedOrGreen = !bRedOrGreen;
		uOldMask = ~uMask;
		old_channel_number = ~channel_number;
		bOldIsZbuffer = !bIsZbuffer;
		uOldZdepth = ~uZdepth;
		bOldAlphaEnabled = !bAlphaEnabled;

		dmaChain.addSrcCntTag(0, VIF_FLUSH());
		dmaChain.endPacket();

		// this program must be on top so clear the cachel

		Vu1::instance()->clearCache();
		Vu1::instance()->getProgram(dmaChain, 1, &VUIndexPass_Begin, &VUIndexPass_End);

		// add micro program

		// mscall init program

		dmaChain.addSrcCntTag();
		dmaChain.addBase(0);
		dmaChain.addOffset(512);
		dmaChain.addStcycl(1,1);

		dmaChain.addMscal(0);   // initialize program

		// set Mask
		dmaChain.stmod(0);


		dmaChain.align32(4, 3);
		dmaChain.add32(VIF_DIRECT(4));
		dmaChain.add128(GIFTAG(3, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
		dmaChain.add128(GS_XYOFFSET(0, 0), GS_REG(XYOFFSET_2));
		dmaChain.add128(GS_SCISSOR(0, 512-1, 0, 448-1), GS_REG(SCISSOR_2));  // TODO: Fix me with proper size
		dmaChain.add128(GS_TEX1(0, 0, 0, 0, 0, 0, 0), GS_REG(TEX1_2));
		dmaChain.endPacket();
	}

	dmaChain.addSrcCntTag();
	// if Palette has been set
	if(pPhysPaletteUploadChain != NULL)
	{
		dmaChain.add32(VIF_FLUSH());
		dmaChain.align(16);
		dmaChain.endPacket();
		dmaChain.addSrcCallTag(pPhysPaletteUploadChain);
		dmaChain.addSrcCntTag();
	}

	// set new MASK
	if(uOldMask != uMask)
	{
		dmaChain.stcol();

		for(int k=0; k<4; k++) 
			dmaChain.add32(uMask);

		uOldMask = uMask;
	}

	// set CLAMP
	if(bOldRedOrGreen != bRedOrGreen)
	{
		if(pPhysPaletteUploadChain == NULL)
			dmaChain.add32(VIF_FLUSH());

		dmaChain.align32(4, 3);
		dmaChain.add32(VIF_DIRECT(3));
		dmaChain.add128(GIFTAG(2, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
		dmaChain.add128(SCE_GS_SET_CLAMP(3, 0, 0xf7, ((bRedOrGreen)?0:0x8), 0, 0),  GS_REG(CLAMP_2));
		dmaChain.add128(0, GS_REG(TEXFLUSH));

		//ZENIC_INFO("Setting clamp");

		bOldRedOrGreen = bRedOrGreen;
	}
	else
	{
		dmaChain.align32(4, 3);
		dmaChain.add32(VIF_DIRECT(2));
		dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
		dmaChain.add128(0, GS_REG(TEXFLUSH));
	}
    
	// trigger initial settings
	if(old_channel_number != channel_number ||
		bOldIsZbuffer != bIsZbuffer ||
		uOldZdepth != uZdepth ||
		bOldAlphaEnabled != bAlphaEnabled )
	{   
		dmaChain.align32(4,3);
		dmaChain.addUnpack(vif::V4_32, 0, 2, true);
		//dmaChain.add32(VIF_UNPACKRU(vif::V4_32, 2, 0));

		// add giftag
		const u32 abe = (bAlphaEnabled)?1:0;
		const u64 uPrim = GS_PRIM(6, 0, 1, 0, abe, 0, 1, 1, 0);
		dmaChain.add64(GIFTAG(8*8+8*4, 1, 1, uPrim, gif::Packed, 4));
		dmaChain.add64(gif::UV | (gif::XYZ2<<4) | ((gif::UV | (gif::XYZ2<<4))<<8));

		// add information, remaining information will be added in the prebuilt chain
		dmaChain.add32((((bRedOrBlue)?1:0) | ((bIsZbuffer)?2:0)));  // Flag
		dmaChain.add32(uZdepth );               // zbuffer depth
		dmaChain.add32(0);
		dmaChain.add32(0);

		// update settings
		old_channel_number = channel_number;
		bOldIsZbuffer = bIsZbuffer;
		uOldZdepth = uZdepth;
		bOldAlphaEnabled = bAlphaEnabled;
	}
	else
	{
		// add information (-1) means same settings as for last batch
		dmaChain.addUnpack(vif::S_32, 1, 1, true);
		//dmaChain.add32(VIF_UNPACKRU(vif::S_32, 1, 1));
		dmaChain.add32(0xffffffff); // Flag
	}

	dmaChain.align(16);
	dmaChain.endPacket();

	// add call chain
	ZENIC_ASSERT(pPhysCallChain!=NULL);
	dmaChain.addSrcCallTag(pPhysCallChain);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add to chain ////////////

void IncCurPageCount(u32 * pCurPageCount, const u32 uActHorzPages)
{ 
	const u32 uCurPageCount = (*pCurPageCount)+1;
	(*pCurPageCount) = (uCurPageCount>=uActHorzPages) ? (uCurPageCount-uActHorzPages) : uCurPageCount;
	ZENIC_ASSERT((*pCurPageCount)<uActHorzPages);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const u32 PostEffectBuilder::unfoldAddList(Dma& dmaChain, u32 * pVramLoc, const bool bFirst, u32 uCurPageCount, const u32 delivery_number, const u32 uActHorzPages, const u32 uAdder, const u32 uPading, const u32 uVifStartAddr)
{
	u32 uPagesLeft = delivery_number;
	u32 uVifAddr = uVifStartAddr;
	bool bFirstRun = bFirst;
    
	while(uPagesLeft>0)
	{
		const u32 uNumBeforeBorder = (uActHorzPages-uCurPageCount)-((bFirstRun)?0:1);
		bool bNeedStitch = ((uPading > 255) && (uPagesLeft>uNumBeforeBorder));
        
		const u32 uDelivery = (bNeedStitch) ? uNumBeforeBorder:uPagesLeft;
		if(uDelivery>0)
		{
			dmaChain.addUnpack(vif::S_8, uVifAddr, uDelivery, true, true, true);
			//dmaChain.add32(VIF_UNPACKRUMSK(vif::S_8, uDelivery, uVifAddr));

			for(u32 k=0; k<((uDelivery+3)>>2); k++)
			{
				u8 uAdders[] = {0,0,0,0};
				ZENIC_ASSERT(k*4<uDelivery);
				const u32 num_adding = ((uDelivery-k*4)>=4)?4:(uDelivery&3);
				ZENIC_ASSERT(num_adding>0 && num_adding<=4);
				for(u32 i=0; i<num_adding; i++)
				{   
					if(bFirstRun)
					{   uAdders[i] = 0; bFirstRun = false; }
					else
					{
						IncCurPageCount(&uCurPageCount, uActHorzPages);
						ZENIC_ASSERT(uCurPageCount!=0 || uPading<=255);
						uAdders[i] = (uCurPageCount==0)?uPading:uAdder;
						(*pVramLoc) += (uCurPageCount==0)?uPading:uAdder;
					}
				}
				dmaChain.add32(((((u32) uAdders[3])<<24)|(((u32) uAdders[2])<<16)|(((u32) uAdders[1])<<8)|uAdders[0]));
			}
        
			uVifAddr += (uDelivery*3);
			uPagesLeft -= uDelivery;
		}

		ZENIC_ASSERT(bFirstRun==false);
        
		if(bNeedStitch)
		{
			IncCurPageCount(&uCurPageCount, uActHorzPages);
			ZENIC_ASSERT(uCurPageCount==0);
			ZENIC_ASSERT(uPagesLeft>0);

			dmaChain.addUnpack(vif::S_32, uVifAddr, 1, true, true, true);
			//dmaChain.add32(VIF_UNPACKRUMSK(vif::S_32, 1, uVifAddr));
			dmaChain.add32(uPading);
        
			(*pVramLoc) += uPading;
			uVifAddr += 3;
			--uPagesLeft;
		}
	}

	// return page_number % uActHorzPages
	return uCurPageCount;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build Palette upload chain

void PostEffectBuilder::buildPaletteUploadChain(u8* dmaMem, const u32 uVRamPaletteLocation, const u32 uSrcColors[256],
												bool swizzle)
{
	Dma dmaChain(dmaMem, 2 * 1024);

	dmaChain.addSrcRetTag(0, VIF_DIRECT(64+5)); // +64 because of pal

	// swizzle colors

	u32 uDstColors[256];
	for(int p=0; p<256; p++)
	{
		if (swizzle)
			uDstColors[(p & 231) + ((p&8)<<1) + ((p&16)>>1)] = uSrcColors[p];
		else
			uDstColors[p] = uSrcColors[p];
	}

	const u32 uQPal = (16*16)/4;

	dmaChain.add128(GIFTAG(3, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);    
	dmaChain.add128(GS_BITBLTBUF(0, 0, 0, uVRamPaletteLocation*32, 1, 0),GS_REG(BITBLTBUF));    
	dmaChain.add128(GS_TRXREG(16, 16), GS_REG(TRXREG));
	dmaChain.add128(GS_TRXDIR(0), GS_REG(TRXDIR));
	dmaChain.add128(GIFTAG(uQPal, gif::Eop, 0, 0, gif::Image, 0), 0);

	for (uint p = 0; p < 256; ++p)
		dmaChain.add32(uDstColors[p]);
    
	// terminate and return physical location of chain

	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

