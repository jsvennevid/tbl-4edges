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

#ifndef zenic_ps2_PostEffectBuilder_h
#define zenic_ps2_PostEffectBuilder_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forwards
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Dma;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PostEffectBuilder
{
public:

	static void buildPaletteUploadChain(u8* dmaMem, const u32 uVRamPaletteLocation, const u32 uSrcColors[256], bool swizzle = true);
	static void buildIndexChain(u8* dmaMem, const u32 uVRamSrc, const u32 uVRamDst, const u32 uVRamZbufArg, 
									const u32 uVRamPaletteLocation, 
									const s32 x, const s32 y, const s32 dimx, const s32 dimy, 
									const bool bEnableZwrite=false, const u32 uBufferWidth = 512);

	static void uploadIndexChain(Dma& dmaChain, const void * pPhysCallChain, const int channel_number, const u32 uMask,
							 const bool bIsZbuffer, const u32 uZdepth, const bool bAlphaEnabled=false,
							 const void * pPhysPaletteUploadChain = 0);

private:

	static const u32 unfoldAddList(Dma& dmaChain, u32 * pVramLoc, const bool bFirst, u32 uCurPageCount, 
								   const u32 delivery_number, const u32 uActHorzPages, const u32 uAdder, 
								   const u32 uPading, const u32 uVifStartAddr);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}



#endif //zenic_ps2_PostEffectBuilder_h



