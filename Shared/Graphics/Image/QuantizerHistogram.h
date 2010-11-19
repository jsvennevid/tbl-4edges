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
