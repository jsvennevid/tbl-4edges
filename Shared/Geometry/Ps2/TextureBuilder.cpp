

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureBuilder::swizzle8to32(void* swizTexels, void* texels, s32 width, s32 height)
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

void TextureBuilder::swizzle4to32(void* swizTexels, void* texels, s32 width, s32 height)
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
			u8 uPen = ((u8 *) texels)[y*width+x];

			// swizzle
			s32 pageX = x & (~0x7f);
			s32 pageY = y & (~0x7f);

			s32 pages_horz = (width+127)/128;
			s32 pages_vert = (height+127)/128;

			s32 page_number = (pageY/128)*pages_horz + (pageX/128);

			s32 page32Y = (page_number/pages_vert)*32;
			s32 page32X = (page_number%pages_vert)*64;

			s32 page_location = page32Y*height*2 + page32X*4;

			s32 locX = x & 0x7f;
			s32 locY = y & 0x7f;	

			s32 block_location = ((locX&(~0x1f))>>1)*height + (locY&(~0xf))*2;
			u32 swap_selector = (((y+2)>>2)&0x1)*4;
			s32 posY = (((y&(~3))>>1) + (y&1))&0x7;

			s32 column_location = posY*height*2 + ((x+swap_selector)&0x7)*4;

			s32 byte_num = (x>>3)&3;	// 0,1,2,3
			s32 bits_set = (y>>1)&1;	// 0,1		(lower/upper 4 bits)

			u8 &setPix = ((u8 *) swizTexels)[page_location + block_location + column_location + byte_num];
			setPix = (setPix & (-bits_set)) | (uPen<<(bits_set*4));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TextureBuilder::swizzle16to32(void* swizTexels, void* texels, s32 width, s32 height)
{
	// this function works for the following resolutions
	// Width:	16, 32, 48, 64, any multiple of 64 smaller then or equal to 4096
	// Height:	8, 16, 24, 32, 40, 48, 56, 64, any multiple of 64 smaller then or equal to 4096

	// the texels must be uploaded as a 32bit texture
	// width_32bit = height_16bit
	// height_32bit = width_16bit / 2
	// remember to adjust the mapping coordinates when
	// using a dimension which is not a power of two

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			u16 uCol = ((u16 *) texels)[y*width+x];

			s32 pageX = x & (~0x3f);
			s32 pageY = y & (~0x3f);

			s32 pages_horz = (width+63)/64;
			s32 pages_vert = (height+63)/64;

			s32 page_number = (pageY/64)*pages_horz + (pageX/64);

			s32 page32Y = (page_number/pages_vert)*32;
			s32 page32X = (page_number%pages_vert)*64;

			s32 page_location = (page32Y*height + page32X)*2;

			s32 locX = x & 0x3f;
			s32 locY = y & 0x3f;	

			s32 block_location = (locX&(~0xf))*height + (locY&(~0x7))*2;
			s32 column_location = ((y&0x7)*height + (x&0x7))*2;

			s32 short_num = (x>>3)&1;	// 0,1

			((short *) swizTexels)[page_location + block_location + column_location + short_num] = uCol;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TextureBuilder::isSwizzable4(s32 width, s32 height)
{
	// Width:	32, 64, 96, 128, any multiple of 128 smaller then or equal to 4096
	// Height:	16, 32, 48, 64, 80, 96, 112, 128, any multiple of 128 smaller then or equal to 4096

	bool b4HorzSwizzable = ((width&0x7f)==0) || (((width&0x1f)==0) && width<0x80);
	bool b4VertSwizzable = ((height&0x7f)==0) || (((height&0xf)==0) && height<0x80);

	return b4HorzSwizzable && b4VertSwizzable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TextureBuilder::isSwizzable8(s32 width, s32 height)
{
	// Width:	any multiple of 16 smaller then or equal to 4096
	// Height:	any multiple of 4 smaller then or equal to 4096

	bool b8HorzSwizzable = (width&0xf)==0;
	bool b8VertSwizzable = (height&0x3)==0;

	return b8HorzSwizzable && b8VertSwizzable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TextureBuilder::isSwizzable16(s32 width, s32 height)
{
	// Width:	16, 32, 48, 64, any multiple of 64 smaller then or equal to 4096
	// Height:	8, 16, 24, 32, 40, 48, 56, 64, any multiple of 64 smaller then or equal to 4096

	bool b16HorzSwizzable = ((width&0x3f)==0) || (((width&0xf)==0) && width<0x40);
	bool b16VertSwizzable = ((height&0x3f)==0) || (((height&0x7)==0) && height<0x40);

	return b16HorzSwizzable && b16VertSwizzable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/
