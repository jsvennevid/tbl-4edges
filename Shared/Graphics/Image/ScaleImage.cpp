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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ScaleImage.h"
#include <Shared/Base/Debug/Assert.h>
#include <string.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ScaleImage::run(Pixel* pSrc, uint uiSrcWidth, uint uiSrcHeight, Pixel* pDest, uint uiDestWidth, uint uiDestHeight)
{
	// TODO: Rewrite to fit new coding stanard.

	unsigned char* pkWidth = CreateFilter( uiSrcWidth, uiDestWidth );
	ZENIC_ASSERT( pkWidth );

	unsigned char* pkHeight = CreateFilter( uiSrcHeight, uiDestHeight );
	ZENIC_ASSERT( pkHeight );

	if( !pkWidth || !pkHeight )
		return false;

	Kernel* pkXKernel = (Kernel*)pkWidth;
	Kernel* pkYKernel = (Kernel*)pkHeight;

	Source* pXLimit = (Source*)(pkWidth + pkXKernel->m_uiSize);
	Source* pYLimit = (Source*)(pkHeight + pkYKernel->m_uiSize);

	// setup row table

	unsigned int uiActive = 0;
	FilterRow** pkActive = new FilterRow*[uiDestHeight];
	FilterRow* pkRow = 0;
	ZENIC_ASSERT( pkActive );

	memset( pkActive, 0, uiDestHeight * sizeof(FilterRow*) );

	Pixel* pkSourceRow = 0;
	unsigned int uiSourceRow = 0;

	FilterRow* pkRowCache = 0;

	for( Source* pkYSource = pkYKernel->m_kSource; pkYSource < pYLimit; pkYSource = (Source*)(((unsigned char*)pkYSource) + pkYSource->m_uiSize) )
	{
		Dest* pkYDestLimit = (Dest*)(((unsigned char*)pkYSource) + pkYSource->m_uiSize);
		Dest* pkYDest;

		if( pkYSource->m_kDest < pkYDestLimit )
		{
			for( pkYDest = pkYSource->m_kDest; pkYDest < pkYDestLimit; pkYDest++ )
			{
				// create rows of pixels needed for accumulating to destination

				if( !pkActive[pkYDest->m_uiDest] )
				{
					// create new if none in cache

					if( !pkRowCache )
					{
						pkRowCache = new FilterRow;
						ZENIC_ASSERT( pkRowCache );

						pkRowCache->m_pkPixels = new Pixel[uiDestWidth];
						ZENIC_ASSERT( pkRowCache->m_pkPixels );
					}

					pkRow = pkRowCache;
					pkRowCache = pkRowCache->m_pkNext;

					memset( pkRow->m_pkPixels, 0, uiDestWidth * sizeof(Pixel) );
					pkRow->m_fWeight = 0;
					pkRow->m_pkNext = 0;

					pkActive[pkYDest->m_uiDest] = pkRow;;
					uiActive++;
				}
			}
		}

		// get source row

		pkSourceRow = &pSrc[uiSrcWidth * uiSourceRow];

		Pixel* pkSourcePixel = pkSourceRow;

		// process row

		for( Source* pXSource = pkXKernel->m_kSource; pXSource < pXLimit; pXSource = (Source*)(((unsigned char*)pXSource) + pXSource->m_uiSize) )
		{
			Dest* pkXDestLimit = (Dest*)(((unsigned char*)pXSource) + pXSource->m_uiSize);

			for( pkYDest = pkYSource->m_kDest; pkYDest < pkYDestLimit; pkYDest++ )
			{
				pkRow = pkActive[pkYDest->m_uiDest];

				for( Dest* pkXDest = pXSource->m_kDest; pkXDest < pkXDestLimit; pkXDest++ )
				{
					float fWeight = pkYDest->m_fWeight * pkXDest->m_fWeight;

					pkRow->m_pkPixels[pkXDest->m_uiDest].r += pkSourcePixel->r * fWeight;
					pkRow->m_pkPixels[pkXDest->m_uiDest].g += pkSourcePixel->g * fWeight;
					pkRow->m_pkPixels[pkXDest->m_uiDest].b += pkSourcePixel->b * fWeight;
					pkRow->m_pkPixels[pkXDest->m_uiDest].a += pkSourcePixel->a * fWeight;
					pkRow->m_pkPixels[pkXDest->m_uiDest].luma += pkSourcePixel->luma * fWeight;
				}
			}

			pkSourcePixel++;
		}

		// store completed accumulation rows in dest

		for( pkYDest = pkYSource->m_kDest; pkYDest < pkYDestLimit; pkYDest++ )
		{
			pkRow = pkActive[pkYDest->m_uiDest];
			pkRow->m_fWeight += pkYDest->m_fWeight;

			if( (pkRow->m_fWeight+0.00001) >= 1.0f )
			{
				for( unsigned int i = 0; i < uiDestWidth; i++ )
					pDest[i + uiDestWidth * pkYDest->m_uiDest] = pkRow->m_pkPixels[i];

				// send row back to the cache
				pkActive[pkYDest->m_uiDest] = 0;
				pkRow->m_pkNext = pkRowCache;
				pkRowCache = pkRow;

				uiActive--;
			}
		}		

		// advance to next row

		uiSourceRow++;
	}

	ZENIC_ASSERT( !uiActive );

	delete pkRowCache;

	delete [] pkActive;
	delete [] pkWidth;
	delete [] pkHeight;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ScaleImage::FilterRow::FilterRow()
{
	m_pkPixels	= 0;
	m_pkNext		= 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ScaleImage::FilterRow::~FilterRow()
{
	delete [] m_pkPixels;
	delete m_pkNext;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char* ScaleImage::CreateFilter( unsigned int uiSrcLength, unsigned int uiDestLength )
{
	ZENIC_ASSERT( uiSrcLength && uiDestLength );

	if( !uiSrcLength || !uiDestLength )
		return 0;

	unsigned int i,j;

	// compute filter scaling

	float fScale = float(uiDestLength) / float(uiSrcLength);
	float	fInvScale = 0.5f / fScale;

	// compute how much memory we will need

	unsigned int uiMemSize = (sizeof(Kernel)-sizeof(Source)) + (sizeof(Source)-sizeof(Dest)) + sizeof(Dest);

	for( i = 0; i < uiSrcLength; i++ )
	{
		float fSource		= float(i) - 0.5f;
		float fDestMin	= fSource * fScale;
		float fDestMax	= fDestMin + fScale;

		uiMemSize += (sizeof(Source)-sizeof(Dest)) + sizeof(Dest) + (unsigned int)(fDestMax - fDestMin + 1) * sizeof(Dest) * 2;
	}

	unsigned char* pkKernel = new unsigned char[uiMemSize];
	ZENIC_ASSERT(pkKernel);

	// start setting up filter

	unsigned int uiSize = (sizeof(Kernel)-sizeof(Source));

	unsigned int uiSum = 0;
	float fSumWeight = 0;

	for( i = 0; i < uiSrcLength; i++ )
	{
		unsigned int uiSourceSize = uiSize;
		Source* pkSource = (Source*)(pkKernel + uiSize);

		// make sure we do not overrun the buffer
		uiSize += (sizeof(Source) - sizeof(Dest));
		ZENIC_ASSERT( uiSize <= uiMemSize );

		for( j = 0; j < 2; j++ )
		{
			// compute source pixel boundary

			float fSource = float(i+j) - 0.5f;

			float fDestMin = fSource * fScale;
			float fDestMax = fDestMin + fScale;

			// clamp filter to border edges
			fDestMin = fDestMin < 0.0f ? 0.0f : fDestMin;
			fDestMax = fDestMax > float(uiDestLength) ? float(uiDestLength) : fDestMax;

			// compute dest pixel location
			unsigned int uiDest = (unsigned int)floorf(fDestMin);

			while(float(uiDest) < fDestMax)
			{
				float fDestStart = float(uiDest);
				float fDestEnd = fDestStart + 1.0f;

				if( uiSum != uiDest )
				{
					if(fSumWeight > 0.00001)
					{
							Dest *pkDest = (Dest*)(pkKernel + uiSize);

							uiSize += sizeof(Dest);
							ZENIC_ASSERT( uiSize <= uiMemSize );

							pkDest->m_uiDest = uiSum;
							pkDest->m_fWeight = fSumWeight;
					}

					fSumWeight = 0.0f;
					uiSum = uiDest;
				}

				fDestStart = fDestStart < fDestMin ? fDestMin : fDestStart;
				fDestEnd = fDestEnd > fDestMax ? fDestMax : fDestEnd;

				float fWeight;

        fWeight = (fSource < 0.0f) ? 1.0f : (fSource + 1.0f >= float(uiSrcLength)) ? 0 :
																		(fDestStart + fDestEnd) * fInvScale - fSource;

        fSumWeight += (fDestEnd - fDestStart) * (j ? 1.0f - fWeight : fWeight);
				uiDest++;
			}
		}

    if(fSumWeight > 0.00001)
    {
			Dest *pkDest = (Dest*)(pkKernel + uiSize);

			uiSize += sizeof(Dest);
			ZENIC_ASSERT( uiSize <= uiMemSize );

			pkDest->m_uiDest = uiSum;
			pkDest->m_fWeight = fSumWeight;
    }

    fSumWeight = 0.0f;

    pkSource->m_uiSize = uiSize - uiSourceSize;
  }

	((Kernel*)pkKernel)->m_uiSize = uiSize;

	return pkKernel;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
