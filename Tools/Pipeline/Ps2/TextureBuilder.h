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

#ifndef zenic_ps2_TextureBuilder_h
#define zenic_ps2_TextureBuilder_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Base/Types.h>
#include <Shared/Hardware/Ps2/Gs.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Dma;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TextureBuilder
{
public:
	inline TextureBuilder() {};
	static TextureBuilder& instance();
    
	Texture* createTexture(u16 width, u16 height, gs::Psm source, gs::Psm dest,
									  u16 MipLevels, const u8* data, uint vramLoc = 450);


	//bool BuildChainNew(u32* dmaData, Texture* texture, u16 mipLevels, s32 dimX, s32 dimY, s32 mode, uint vramLoc = 450);
	static u32 totalSize(u32 width, u32 height, u32 miplevels);

	// TODO: Make access functions

	uint m_palOffsetCount;
	u32 m_palOffsets[64];
	uint m_imageOffsetCount;
	u32 m_imageOffsets[64];

private :


	Texture* create8(u16 width, u16 height, gs::Psm source, u16 mipLevels, const u8* data, uint vramLoc = 450);
	Texture* create4(u16 width, u16 height, gs::Psm source, u16 mipLevels, const u8* data, uint vramLoc = 450);

	void swizzleTexture(Texture* textureMap, const u8* image, const u32* pal, u16 mipLevels, 
						u16 width, u16 height, gs::Psm psm);

	void buildChain(Texture* textureMap, u8* image, u32* pal, u16 mipLevels, u16 width, 
					u16 height, gs::Psm psm);

	uint calcLog(uint value);

	// Swizzle functions

	void swizzle4to32(void* ZENIC_RESTRICT swizTexels, const void* ZENIC_RESTRICT texels, s32 width, s32 height);
	void swizzle8to32(void* ZENIC_RESTRICT swizTexels, const void* ZENIC_RESTRICT texels, s32 width, s32 height);
	void swizzle16to32(void* ZENIC_RESTRICT swizTexels, const void* ZENIC_RESTRICT texels, s32 width, s32 height);

	bool isSwizzable4(s32 width, s32 height);
	bool isSwizzable8(s32 width, s32 height);
	bool isSwizzable16(s32 width, s32 height);
	bool isSwizzable(s32 width, s32 height, s32 mode);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}


#endif //zenic_ps2_TextureBuilder_h

