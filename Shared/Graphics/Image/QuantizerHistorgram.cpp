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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "QuantizerHistogram.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const float QuantizerHistogram::m_ditherMatrix[16] = 
{
	-0.375, -0.250, -0.750,  0.125,
	 0.625, -1.250,  0.250, -0.875,
	-0.625, -0.000, -0.500, -0.125,
	 0.375, -1.000,  0.500, -1.125,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QuantizerHistogram::QuantizerHistogram()
{
	m_pHistogram = 0;
	for(int i = 0; i < 4096; i++)
		m_pHistHash[i] = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QuantizerHistogram::~QuantizerHistogram()
{
 	histogram	*next;
 	while(m_pHistogram != 0)
 	{
 		next = m_pHistogram->next;
 		delete m_pHistogram;
 		m_pHistogram = next;
 	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool QuantizerHistogram::PreProcess()
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool QuantizerHistogram::Process( u8* pImage,u32* pPal,
                                    u32* pRgbaImage,u32 Size, 
                                    u32 Colors )
{
  feedImage( (const unsigned char*)pRgbaImage, Size );
  quantize( (int)Colors );

  // store pal

  memcpy( pPal, (u8*)&m_palette, Colors*4 );

  // map the pixels

  if( Colors > 16 )
  {
    color* p_colors = (color*)pRgbaImage;

    for( u32 i = 0; i < Size; i++ )
      pImage[i] = mapPixel( p_colors[i], 0, 0 );
  }
  else
  {
	// NOTE: Not needed to "pack" pixels here as its handles by swizzler code or other code outside.
	  
	color* p_colors = (color*)pRgbaImage;

	for( u32 i = 0; i < Size; i++ )
      pImage[i] = mapPixel( p_colors[i], 0, 0 );

	  // Code for 4bit
/*
    u32 d = 0;
  
    color* p_colors = (color*)pRgbaImage;

    for( u32 i = 0; i < Size; i+=2, d++ )
    {
      pImage[d] =   mapPixel( p_colors[i+0], 4, 4 ) | 
                  ( mapPixel( p_colors[i+1], 4, 4 ) << 4 ) ;
    }
*/
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool QuantizerHistogram::PostProcess()
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QuantizerHistogram::feedImage(const unsigned char *data, int numPixels)
{
	unsigned char r, g, b, a;
	histogram	*cur;
	int hashValue;
	for(int i = 0; i < numPixels; i++)
	{
		r = *data++; g = *data++; b = *data++; a = *data++;
		hashValue = ((r >> 4) & 15) + ((g >> 4) & 15) * 16 + ((b >> 4) & 15) * 256;
		cur = m_pHistHash[hashValue];
		while(cur != 0 && (r != cur->r || g != cur->g || b != cur->b || a != cur->a))
			cur = cur->nextHash;
		if(cur != 0)
		{
			cur->num++;
		}
   		else
   		{
          cur = new histogram;
          cur->next = m_pHistogram;
          m_pHistogram = cur;
		  cur->nextHash = m_pHistHash[hashValue];
		  m_pHistHash[hashValue] = cur;
          cur->r = r; cur->g = g; cur->b = b; cur->a = a;
          cur->num = 1;
   		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QuantizerHistogram::sumRect(colorRect &rect)
{
	histogram	*cur = m_pHistogram;
	float		sum, sumr, sumg, sumb, suma, sumr2, sumg2, sumb2, suma2, vr, vg, vb, va;
	sum = sumr = sumg = sumb = suma = sumr2 = sumg2 = sumb2 = suma2 = 0;
	
	while(cur != 0)
	{
		if(cur->r >= rect.rmin && cur->r < rect.rmax &&
			cur->g >= rect.gmin && cur->g < rect.gmax &&
			cur->b >= rect.bmin && cur->b < rect.bmax &&
			cur->a >= rect.amin && cur->a < rect.amax)
		{
			sum += cur->num;
			sumr += (float) cur->r * cur->num;
			sumg += (float) cur->g * cur->num;
			sumb += (float) cur->b * cur->num;
			suma += (float) cur->a * cur->num;
			sumr2 += (float) cur->r * cur->r * cur->num;
			sumg2 += (float) cur->g * cur->g * cur->num;
			sumb2 += (float) cur->b * cur->b * cur->num;
			suma2 += (float) cur->a * cur->a * cur->num;
		}
		cur = cur->next;
	}
	
	if(sum > 0)
	{
		rect.r0 = sumr / sum;
		rect.g0 = sumg / sum;
  		rect.b0 = sumb / sum;
		rect.a0 = suma / sum;
  		vr = (sumr2 - sumr * rect.r0);// / sum;
  		vg = (sumg2 - sumg * rect.g0);// / sum;
  		vb = (sumb2 - sumb * rect.b0);// / sum;
		va = (suma2 - suma * rect.a0);// / sum;
  		rect.v = vr + vg + vb + va;
  		if(vr > vg && vr > vb && vr > va)
  			rect.axis = RED;
		else if(vg > vb && vg > va)
			rect.axis = GREEN;
		else if(vb > va)
			rect.axis = BLUE;
		else
			rect.axis = ALPHA;
	}
	else
	{
		rect.r0 = rect.g0 = rect.b0 = rect.a0 = rect.v = 0;
		rect.axis = RED;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QuantizerHistogram::quantize(int numColors)
{
	int i, j, maxi = 0;
	float maxv;
	
	m_numColors = numColors;

	rects[0].rmin = rects[0].gmin = rects[0].bmin = rects[0].amin = 0;
	rects[0].rmax = rects[0].gmax = rects[0].bmax = rects[0].amax = 256;
	
	sumRect(rects[0]);
	
	for(i = 1; i < numColors; i++)
	{
		maxv = 0;
		for(j = 0; j < i; j++)
			if(rects[j].v >= maxv)
			{
				maxv = rects[j].v;
				maxi = j;
			}
			
		rects[i].rmin = rects[maxi].rmin;
		rects[i].gmin = rects[maxi].gmin;
		rects[i].bmin = rects[maxi].bmin;
		rects[i].amin = rects[maxi].amin;
		rects[i].rmax = rects[maxi].rmax;
		rects[i].gmax = rects[maxi].gmax;
		rects[i].bmax = rects[maxi].bmax;
		rects[i].amax = rects[maxi].amax;
		
		switch(rects[maxi].axis)
		{
		case RED:
			rects[i].rmin = rects[maxi].r0;
			rects[maxi].rmax = rects[maxi].r0;
			break;
		case GREEN:
			rects[i].gmin = rects[maxi].g0;
			rects[maxi].gmax = rects[maxi].g0;
			break;
		case BLUE:
			rects[i].bmin = rects[maxi].b0;
			rects[maxi].bmax = rects[maxi].b0;
			break;
		case ALPHA:
			rects[i].amin = rects[maxi].a0;
			rects[maxi].amax = rects[maxi].a0;
			break;
		}

		sumRect(rects[maxi]);
		sumRect(rects[i]);
	}
	
	for(i = 0; i < numColors; i++)
	{
		m_palette[i].r = (unsigned char)rects[i].r0;
		m_palette[i].g = (unsigned char)rects[i].g0;
		m_palette[i].b = (unsigned char)rects[i].b0;
		m_palette[i].a = (unsigned char)rects[i].a0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int QuantizerHistogram::findRect(int r, int g, int b, int a)
{
	int i;
	for(i = 0; i < m_numColors; i++)
	{
		if(r >= rects[i].rmin && r < rects[i].rmax &&
			g >= rects[i].gmin && g < rects[i].gmax &&
			b >= rects[i].bmin && b < rects[i].bmax &&
			a >= rects[i].amin && a < rects[i].amax)
			return i;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char QuantizerHistogram::mapPixel(color &pixel, int x, int y, float maxDither)
{
	float vr, vg, vb, va, d, f;
	int r, g, b, a;
	unsigned char bestIndex;
	int i, e, beste;
	
	r = pixel.r; g = pixel.g; b = pixel.b; a = pixel.a;
	
	colorRect &rect = rects[findRect(r, g, b, a)];

	f = (r - rect.rmin) / (rect.rmax - rect.rmin);
	vr = (rect.r0 - rect.rmin) * (1.0f - f) + (rect.rmax - rect.r0) * f;
	
	f = (g - rect.gmin) / (rect.gmax - rect.gmin);
	vg = (rect.g0 - rect.gmin) * (1.0f - f) + (rect.gmax - rect.g0) * f;

	f = (b - rect.bmin) / (rect.bmax - rect.bmin);
	vb = (rect.b0 - rect.bmin) * (1.0f - f) + (rect.bmax - rect.b0) * f;

	f = (a - rect.amin) / (rect.amax - rect.amin);
	va = (rect.a0 - rect.amin) * (1.0f - f) + (rect.amax - rect.a0) * f;
	
	if(vr > maxDither)
		vr = maxDither;
	if(vg > maxDither)
		vg = maxDither;
	if(vb > maxDither)
		vb = maxDither;
	if(va > maxDither)
		va = maxDither;

	d = (float) m_ditherMatrix[(x & 3) + (y & 3) * 4] + 0.375f;
		
	r += (int)(d * vr);
	g += (int)(d * vg);
	b += (int)(d * vb);
	a += (int)(d * va);
	
	if(r < 0)
		r = 0;
	if(r > 255)
		r = 255;
	if(g < 0)
		g = 0;
	if(g > 255)
		g = 255;
	if(b < 0)
		b = 0;
	if(b > 255)
		b = 255;
	if(a < 0)
		a = 0;
	if(a > 255)
		a = 255;

	beste = (1 << 30);
	bestIndex = 0;
	for(i = 0; i < m_numColors; i++)
	{
		e = (m_palette[i].r - r) * (m_palette[i].r - r) +
			(m_palette[i].g - g) * (m_palette[i].g - g) +
			(m_palette[i].b - b) * (m_palette[i].b - b) +
			(m_palette[i].a - a) * (m_palette[i].a - a);
  		if(e < beste)
  		{
  			bestIndex = (unsigned char)i;
  			beste = e;
    	}
	}

	return bestIndex;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
