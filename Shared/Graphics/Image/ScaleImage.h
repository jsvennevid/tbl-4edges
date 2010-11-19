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

#ifndef zenic_ScaleImage_h
#define zenic_ScaleImage_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ScaleImage
{
public : 

	struct Pixel
	{
		float r;
		float g;
		float b;
		float a;
		float luma;
	};

	static bool run(Pixel* src, uint srcWidth, uint srcHeight, Pixel* dest, uint destWidth, uint destHeight);

private:

	// TODO: Rewrite to correct coding standard

	struct Dest
	{
		unsigned int m_uiDest;
		float m_fWeight;
	};

	struct Source
	{
		unsigned int m_uiSize;
		Dest m_kDest[1];
	};

	struct Kernel
	{
		unsigned int m_uiSize;
		Source m_kSource[1];
	};

	struct FilterRow
	{
		Pixel* m_pkPixels;
		float m_fWeight;
		FilterRow* m_pkNext;

		FilterRow();
		~FilterRow();
	};

	static unsigned char* CreateFilter(uint uiSrcLength, uint uiDestLength);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

#endif
