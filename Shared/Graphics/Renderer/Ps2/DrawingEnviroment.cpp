///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
