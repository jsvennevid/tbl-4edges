///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "TextureBuilder.h"
#include <string.h>

#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Graphics/Image/QuantizerHistogram.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture* TextureBuilder::createTexture(u16 width, u16 height, gs::Psm source, gs::Psm dest, u16 mipLevels, 
									   const u8* data, uint vramLoc)
{
	Texture* textureMap = 0;
	m_palOffsetCount = m_imageOffsetCount = 0;

	switch (dest)
	{
		case gs::CT32 : textureMap = 0; break;
		case gs::CT24 : textureMap = 0; break;
		case gs::CT16 : textureMap = 0; break;
		case gs::T8 : textureMap = create8(width, height, source, mipLevels, data, vramLoc); break;
		case gs::T4 : textureMap = create4(width, height, source, mipLevels, data, vramLoc); break;
	}

  return textureMap;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture* TextureBuilder::create8(u16 width, u16 height, gs::Psm source, u16 mipLevels, const u8* data, uint vramLoc)
{
	Texture* textureMap = new Texture;

	// TODO: Make support for textures smaller than 8kB

	u32 size = totalSize(width, height, mipLevels);
	//ZENIC_ASSERT(size >= 8 * 1024);

	u8* destImage = zenic_new u8[size];
	u32* destPal = zenic_new u32[256];

	// TODO: We only support 1 mipmap level for now, build new levels here if needed

	ZENIC_ASSERT(mipLevels == 1);

	QuantizerHistogram quantizer;
	quantizer.Process(destImage, destPal, (u32*)data, size, 255);

	ZENIC_ASSERT(width >= 64 || height <= 512);
	ZENIC_ASSERT(height >= 128 || height < 512);

	uint bufferWidth = width / 64; 
	if (bufferWidth == 0) 
		bufferWidth = 1;

	uint uploadBufferWidth = (width / 2) / 64;
	if (uploadBufferWidth == 0) 
		uploadBufferWidth = 1;

	textureMap->setUploadParameters(Texture::Parameters(uploadBufferWidth, gs::CT32, 0, width / 2, height / 2));
	textureMap->setDisplayParameters(Texture::Parameters(bufferWidth, gs::T8, 0, calcLog(width), calcLog(height)));
	textureMap->setMipMaps(1);

	swizzleTexture(textureMap, destImage, destPal, mipLevels, width, height, gs::T8);
	//buildUploadChain(textureMap, gs::T8, vramLoc);

	return textureMap;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture* TextureBuilder::create4(u16 width, u16 height, gs::Psm source, u16 mipLevels, const u8* data, uint vramLoc)
{
	Texture* textureMap = new Texture;

	// TODO: Make support for textures smaller than 8kB

	u32 size = totalSize(width, height, mipLevels);

	u8* destImage = zenic_new u8[size];
	u32* destPal = zenic_new u32[16];

	// TODO: We only support 1 mipmap level for now, build new levels here if needed

	ZENIC_ASSERT(mipLevels == 1);

	QuantizerHistogram quantizer;
	quantizer.Process(destImage, destPal, (u32*)data, size, 15);

	ZENIC_ASSERT(width >= 64 || height <= 512);
	ZENIC_ASSERT(height >= 128 || height < 512);

	uint bufferWidth = width / 64; 
	if (bufferWidth == 0) 
		bufferWidth = 1;

	uint uploadBufferWidth = (width / 2) / 64;
	if (uploadBufferWidth == 0) 
		uploadBufferWidth = 1;

	textureMap->setUploadParameters(Texture::Parameters(uploadBufferWidth, gs::CT32, 0, width / 2, height / 4));
	textureMap->setDisplayParameters(Texture::Parameters(bufferWidth, gs::T4, 0, calcLog(width), calcLog(height)));
	textureMap->setMipMaps(1);

	swizzleTexture(textureMap, destImage, destPal, mipLevels, width, height, gs::T4);
	//buildUploadChain(textureMap, gs::T8, vramLoc);

	return textureMap;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureBuilder::swizzleTexture(Texture* textureMap, const u8* image, const u32* pal, u16 mipLevels, u16 width, u16 height, gs::Psm psm)
{
	u32 dmaData[2048];

	Dma dmaChain(dmaData, 2048/4); 

	u32 wholeSize = totalSize(width, height, mipLevels);

	if (psm == gs::T4)
		wholeSize /= 2;

	u8* ZENIC_RESTRICT swizzleImage = zenic_new u8[wholeSize];
	u32* ZENIC_RESTRICT swizzlePal = zenic_new u32[256]; // TODO: Check if 8 or 4bit

	textureMap->setMipMaps(mipLevels);

	// Swizzle the whole texture to 32bit format

	ZENIC_ASSERT_DESC(mipLevels == 1, "No support for mipmap levels above 1");

	for (uint i = 0; i < mipLevels; ++i)
	{
		ZENIC_ASSERT_DESC(isSwizzable(width, height, psm), "Unable to swizzle texture");
		u32 size;

		if (psm == gs::T4)
			size = (width * height) / 2;
		else 
			size = (width * height);

		if (psm == gs::T4) 
			swizzle4to32(swizzleImage, image, width, height);
		else if (psm == gs::T8) 
			swizzle8to32(swizzleImage, image, width, height);
		else 
			swizzle16to32(swizzleImage, image, width, height);

		textureMap->setData(i, DataPtr<u8>(swizzleImage, size));

		swizzleImage += size * 2;
		image += size * 2;
		
		width >>= 1;
		height >>= 1;
	}

	// Swizzle pal

	if (psm == gs::T4) 
	{
		memcpy(swizzlePal, pal, 16*4);
		textureMap->setPalette(DataPtr<u8>((u8*)swizzlePal, 16 * 4));
	}
	else if (psm == gs::T8)
	{
		for (u32 p = 0;  p < 256; ++p)
			swizzlePal[(p & 231) + ((p & 8) << 1) + ((p & 16) >> 1)] = pal[p];

		textureMap->setPalette(DataPtr<u8>((u8*)swizzlePal, 256 * 4));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint TextureBuilder::calcLog(uint value)
{
	uint count = 0;

	while (value >>= 1)
		++count;

	return count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TextureBuilder::isSwizzable(s32 width, s32 height, s32 mode)
{
	return ((mode == gs::T4) && isSwizzable4(width, height)) ||
		   ((mode == gs::T8) && isSwizzable8(width, height)) ||
		   ((mode == gs::CT16) && isSwizzable16(width, height));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureBuilder::swizzle8to32(void* ZENIC_RESTRICT swizTexels, const void* ZENIC_RESTRICT texels, s32 width, s32 height)
{
	// this function works for the following resolutions
	// Width:	any multiple of 16 smaller then or equal to 4096
	// Height:	any multiple of 4 smaller then or equal to 4096

	// the texels must be uploaded as a 32bit texture
	// width_32bit = width_8bit / 2
	// height_32bit = height_8bit / 2
	// remember to adjust the mapping coordinates when
	// using a dimension which is not a power of two

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			u8 uPen = ((u8 *) texels)[y*width+x];

			s32 block_location = (y&(~0xf))*width + (x&(~0xf))*2;
			u32 swap_selector = (((y+2)>>2)&0x1)*4;
			s32 posY = (((y&(~3))>>1) + (y&1))&0x7;
			s32 column_location = posY*width*2 + ((x+swap_selector)&0x7)*4;

			s32 byte_num = ((y>>1)&1) + ((x>>2)&2);	// 0,1,2,3

			//((u8 *) pSwizTexels)[(y*width)+x] = uPen;
			((u8 *) swizTexels)[block_location + column_location + byte_num] = uPen;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureBuilder::swizzle4to32(void* ZENIC_RESTRICT swizTexels, const void* ZENIC_RESTRICT texels, s32 width, s32 height)
{
	// this function works for the following resolutions
	// Width:	32, 64, 96, 128, any multiple of 128 smaller then or equal to 4096
	// Height:	16, 32, 48, 64, 80, 96, 112, 128, any multiple of 128 smaller then or equal to 4096

	// the texels must be uploaded as a 32bit texture
	// width_32bit = height_4bit / 2
	// height_32bit = width_4bit / 4
	// remember to adjust the mapping coordinates when
	// using a dimension which is not a power of two

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// get the pen
			u8 uPen = ((u8*) texels)[(y * width) + x];

			// swizzle
			s32 pageX = x & (~0x7f);
			s32 pageY = y & (~0x7f);

			s32 pagesHorz = (width + 127) / 128;
			s32 pagesVert = (height + 127) / 128;

			s32 pageNumber = (pageY / 128) * pagesHorz + (pageX / 128);

			s32 page32Y = (pageNumber / pagesVert) * 32;
			s32 page32X = (pageNumber % pagesVert) * 64;

			s32 pageLocation = page32Y * height * 2 + page32X * 4;

			s32 locX = x & 0x7f;
			s32 locY = y & 0x7f;	

			s32 blockLocation = ((locX & (~0x1f)) >> 1) * height + (locY & (~0xf)) * 2;
			u32 swapSelector = (((y + 2) >> 2) & 0x1) * 4;
			s32 posY = (((y & (~3)) >> 1) + (y & 1)) & 0x7;

			s32 columnLocation = posY * height * 2 + ((x + swapSelector) & 0x7) * 4;

			s32 byteNum = (x >> 3) & 3;	// 0,1,2,3
			s32 bitsSet = (y >> 1) & 1;	// 0,1		(lower/upper 4 bits)

			u8 &setPix = ((u8*) swizTexels)[pageLocation + blockLocation + columnLocation + byteNum];
			setPix = (setPix & (-bitsSet)) | (uPen << (bitsSet * 4));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureBuilder::swizzle16to32(void* ZENIC_RESTRICT swizTexels, const void* ZENIC_RESTRICT texels, s32 width, s32 height)
{
	// this function works for the following resolutions
	// Width:	16, 32, 48, 64, any multiple of 64 smaller then or equal to 4096
	// Height:	8, 16, 24, 32, 40, 48, 56, 64, any multiple of 64 smaller then or equal to 4096

	// the texels must be uploaded as a 32bit texture
	// width_32bit = height_16bit
	// height_32bit = width_16bit / 2
	// remember to adjust the mapping coordinates when
	// using a dimension which is not a power of two

	for (uint y = 0; y < (uint)height; ++y)
	{
		for (uint x = 0; x < (uint)width; ++x)
		{
			u16 uCol = ((u16*) texels)[(y * width) + x];

			s32 pageX = x & (~0x3f);
			s32 pageY = y & (~0x3f);

			s32 pagesHorz = (width + 63) / 64;
			s32 pagesVert = (height + 63) / 64;

			s32 pageNumber = (pageY / 64) * pagesHorz + (pageX / 64);

			s32 page32Y = (pageNumber / pagesVert) * 32;
			s32 page32X = (pageNumber % pagesVert) * 64;

			s32 pageLocation = (page32Y * height + page32X) * 2;

			s32 locX = x & 0x3f;
			s32 locY = y & 0x3f;	

			s32 blockLocation = (locX & (~0xf)) * height + (locY & (~0x7)) * 2;
			s32 columnLocation = ((y & 0x7) * height + (x&0x7)) * 2;

			s32 shortNum = (x >> 3) & 1;	// 0,1

			((short *) swizTexels)[pageLocation + blockLocation + columnLocation + shortNum] = uCol;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TextureBuilder::isSwizzable4(s32 width, s32 height)
{
	// Width:	32, 64, 96, 128, any multiple of 128 smaller then or equal to 4096
	// Height:	16, 32, 48, 64, 80, 96, 112, 128, any multiple of 128 smaller then or equal to 4096

	bool horzSwizzable = ((width & 0x7f) == 0) || (((width & 0x1f)==0) && width < 0x80);
	bool vertSwizzable = ((height & 0x7f) == 0) || (((height & 0xf)==0) && height < 0x80);

	return horzSwizzable && vertSwizzable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TextureBuilder::isSwizzable8(s32 width, s32 height)
{
	// Width:	any multiple of 16 smaller then or equal to 4096
	// Height:	any multiple of 4 smaller then or equal to 4096

	bool horzSwizzable = (width & 0xf) == 0;
	bool vertSwizzable = (height & 0x3) == 0;

	return horzSwizzable && vertSwizzable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TextureBuilder::isSwizzable16(s32 width, s32 height)
{
	// Width:	16, 32, 48, 64, any multiple of 64 smaller then or equal to 4096
	// Height:	8, 16, 24, 32, 40, 48, 56, 64, any multiple of 64 smaller then or equal to 4096

	bool horzSwizzable = ((width & 0x3f) == 0) || (((width & 0xf) == 0) && width < 0x40);
	bool vertSwizzable = ((height & 0x3f) == 0) || (((height & 0x7) == 0) && height < 0x40);

	return horzSwizzable && vertSwizzable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u32 TextureBuilder::totalSize(u32 width, u32 height, u32 miplevels)
{	
	u32 size = 0;
	width <<= 1;	
	height <<= 1;

	do 	
	{		
		width >>= 1;		
		height >>= 1;		
		miplevels--;
		size += width*height;

		if (miplevels == 0)			
			break;	
	}
	while(width>=8 && height>=8);

	return size;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}


