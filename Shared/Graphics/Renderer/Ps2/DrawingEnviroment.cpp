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
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Graphics/Renderer/Ps2/DrawingEnviroment.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Vif.h>
#include <Shared/Hardware/Ps2/Gif.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawingEnviroment::initHeader()
{
	m_dmaData[(0*2)+0] = END_TAG(11);	// we use this area for dmatag/viftags if needed
	m_dmaData[(0*2)+1] = 0; //((u64)(VIF_CODE(VIF_DIRECT, 0, 10))<<32);
	
	m_dmaData[(1*2)+0] = GIFTAG(10, gif::Eop, 0, 0, gif::Packed, 1);
	m_dmaData[(1*2)+1] = gif::A_D;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawingEnviroment::setDefault()
{
	m_dmaData[(2*2)+0] = GS_PRMODECONT(1);  
	m_dmaData[(2*2)+1] = GS_REG(PRMODECONT);
											 
	m_dmaData[(3*2)+0] = GS_FRAME(0, 8, 0, 0);  
	m_dmaData[(3*2)+1] = GS_REG(FRAME_1);

	m_dmaData[(4*2)+0] = GS_ZBUF(224, gs::Z24, 0);  
	m_dmaData[(4*2)+1] = GS_REG(ZBUF_1);
											 
	m_dmaData[(5*2)+0] = GS_SCISSOR(0, 512-1, 0, 448-1);  
	m_dmaData[(5*2)+1] = GS_REG(SCISSOR_1);

	m_dmaData[(6*2)+0] = GS_XYOFFSET((2048) << 4, (2048) << 4);  
	m_dmaData[(6*2)+1] = GS_REG(XYOFFSET_1);

	m_dmaData[(7*2)+0] = GS_TEST(0, gs::alpha::Always, 0xff, gs::alpha::Keep,  
								 0, 0, 1, gs::zbuffer::Always); 
	m_dmaData[(7*2)+1] = GS_REG(TEST_1);
											 
	m_dmaData[(8*2)+0] = GS_COLCLAMP(gs::ColorClamp);
	m_dmaData[(8*2)+1] = GS_REG(COLCLAMP);

	m_dmaData[(9*2)+0] = GS_DIMX(-4, 2, -3, 3, 0, -2, 1, -1, -3, 3, -4, 2, 1, -1, 0, -2);  
	m_dmaData[(9*2)+1] = GS_REG(DIMX);
											 
	m_dmaData[(10*2)+0] = GS_DTHE(0);  
	m_dmaData[(10*2)+1] = GS_REG(DTHE);

	m_dmaData[(11*2)+0] = GS_TEX1(0, 0, 1, 1, 0, 0, 0);  
	m_dmaData[(11*2)+1] = GS_REG(TEX1_1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
