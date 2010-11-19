///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_QuantizerHistogram_h
#define zenic_QuantizerHistogram_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QuantizerHistogram
{
public:

	QuantizerHistogram();
	~QuantizerHistogram();

	bool PreProcess();
	bool Process(u8* pImage,u32* pPal,u32* pRgbaImage,
				 u32 Size, u32 Colors);
	bool PostProcess();
	
private:

	struct color
	{
		unsigned char r, g, b, a;
	};

	// feed image data to the quantizer for palette creation
	// the histogram is generated from this, it can be called mutliple times
	// (eg. for mipmaps, or several images that should get the same palette)
	// the size of the input data is expected to be numPixels*4

	void feedImage(const unsigned char *data, int numPixels);

	// do the color reduction
	// after this use the getPalette call to retrieve the palette and
	// mapPixels to map to color indizes

	void quantize(int numColors = 256);

	// retrieve the palette, only valid after calling quantize
	const color	*getPalette() const { return m_palette; }

	// map a truecolor pixel to a color index
	// the x and y parameters are used for the ordered dither
	// always passing 0, 0 effectively disables the dithering

	unsigned char mapPixel(color &pixel, int x = 0, int y = 0, float maxDither = 16.0f);

	enum colorAxis { RED, GREEN, BLUE, ALPHA };

	struct histogram
	{
		unsigned char r, g, b, a;
		int num;
		histogram *next;
		histogram *nextHash;
	};

	struct colorRect
	{
		float rmin, rmax, gmin, gmax, bmin, bmax, amin, amax;
		float r0, b0, g0, a0, v;
		colorAxis axis;
	};

	void       sumRect(colorRect &rect);
	int			   findRect(int r, int g, int b, int a);

	histogram* m_pHistogram;
	histogram* m_pHistHash[4096];
	color		   m_palette[256];
	colorRect	 rects[256];
	int			   m_numColors;

	static const float m_ditherMatrix[16];


};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif //zenic_QuantizerHistogram_h
