///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MasterChain.h"
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Memory/HeapAllocator.h>

#ifdef PS2_EMU
#include <Shared/External/ps2emu/dma.h>
#include <Shared/External/ps2emu/parser.h>
#include <Shared/External/ps2emu/vif1.h>
#include <Shared/External/ps2emu/vu.h>
#include <Shared/External/ps2emu/gif.h>
#endif

namespace zenic
{
	namespace ps2
	{


#ifdef PS2_EMU
ps2emu::Vu* m_pVu1;
ps2emu::Parser* m_pParser;
ps2emu::Gif* m_pGif;
ps2emu::Vif1* m_pVif1;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MasterChain::MasterChain(uint size) : m_size(size), m_flip(false)
{
#ifdef PS2_EMU
	m_pVu1 = new ps2emu::Vu(ps2emu::VPU_1);
	m_pParser = new ps2emu::Parser(m_pVu1);
	m_pParser->LoadInstructions("c:\\temp\\instructions.txt");
	m_pGif = new ps2emu::Gif();
	m_pVif1 = new ps2emu::Vif1(m_pParser, m_pVu1);
#endif

	//m_dmaBuffer1 = new u8[size];
	//m_dmaBuffer2 = new u8[size];

	//m_dmaBuffer1 = new u8[size];
	//m_dmaBuffer2 = new u8[size];

	HeapAllocator temp;

	m_dmaBuffer1 = (u8*)temp.allocate(m_size);
	m_dmaBuffer2 = (u8*)temp.allocate(m_size);

	ZENIC_INFO(m_dmaBuffer1);
	ZENIC_INFO(m_dmaBuffer2);

	flip();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MasterChain::~MasterChain()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MasterChain::kick()
{
#ifdef PS2_EMU
	ps2emu::Dma dma;
	
	dma.openMemory(reinterpret_cast<const char*>(m_vif1Chain.getData()), m_vif1Chain.calcSize());
	dma.processAll(m_pVif1);

	static bool dump = false;

	if (dump)
	{
		m_pVu1->dump("c:\\temp\\vumicromem", "c:\\temp\\vumem");
		dump = false;
	}

#else
	m_vif1Chain.sendChainVif1();
#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MasterChain::flip()
{
	if (m_flip)
		m_vif1Chain.setData(m_dmaBuffer1, m_size);
	else
		m_vif1Chain.setData(m_dmaBuffer2, m_size);

	m_flip = !m_flip;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}
}
