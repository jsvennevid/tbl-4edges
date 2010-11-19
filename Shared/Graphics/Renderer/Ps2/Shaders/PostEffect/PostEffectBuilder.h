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



