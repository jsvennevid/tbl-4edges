///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Hardware/Ps2/DmaSimulator.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Io/File.h>
#include <stdio.h>

#ifdef PS2_EMU
#include <Shared/External/ps2emu/dma.h>
#include <Shared/External/ps2emu/parser.h>
#include <Shared/External/ps2emu/vif1.h>
#include <Shared/External/ps2emu/vu.h>
#include <Shared/External/ps2emu/gif.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only used local for dmadebug
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define D1_ASR0  ((volatile u32 *)(0x10009040))
#define D1_ASR1  ((volatile u32 *)(0x10009050))

typedef struct st_D_STAT
{
  unsigned CIS0: 1;	// Channel interrupt status
  unsigned CIS1: 1;	// Channel interrupt status
  unsigned CIS2: 1;	// Channel interrupt status
  unsigned CIS3: 1;	// Channel interrupt status
  unsigned CIS4: 1;	// Channel interrupt status
  unsigned CIS5: 1;	// Channel interrupt status
  unsigned CIS6: 1;	// Channel interrupt status
  unsigned CIS7: 1;	// Channel interrupt status
  unsigned CIS8: 1;	// Channel interrupt status
  unsigned CIS9: 1;	// Channel interrupt status
  unsigned p0  : 3;
  unsigned SIS : 1;	// DAM Stall interrupt status
  unsigned MEIS: 1;	// Memory FIFO empty interrupt status
  unsigned BEIS: 1;	// BUSERR interrupt status
  unsigned CIM0: 1;	// Channel interrupt mask
  unsigned CIM1: 1;	// Channel interrupt mask
  unsigned CIM2: 1;	// Channel interrupt mask
  unsigned CIM3: 1;	// Channel interrupt mask
  unsigned CIM4: 1;	// Channel interrupt mask
  unsigned CIM5: 1;	// Channel interrupt mask
  unsigned CIM6: 1;	// Channel interrupt mask
  unsigned CIM7: 1;	// Channel interrupt mask
  unsigned CIM8: 1;	// Channel interrupt mask
  unsigned CIM9: 1;	// Channel interrupt mask
  unsigned p1  : 3;
  unsigned SIM : 1;	// DMA Stall interrupt mask
  unsigned MEIM: 1;	// Memory FIFO empty interrupt mask
  unsigned p2  : 1;
} sD_STAT;

typedef struct st_D_CTRL
{
  unsigned DMAE: 1;	// DMA enable
  unsigned RELE: 1;	// release signal enable
  unsigned MFD : 2;	// Memory FIFO drain
  unsigned STS : 2;	// Stall source channel
  unsigned STD : 2;	// Stall drain channel
  unsigned RCYC: 3;	// Release Cycle
  unsigned p0  :21;
} sD_CTRL;

typedef struct st_D_CHCR 
{
  unsigned DIR: 1;	// Direction
  unsigned p0 : 1;
  unsigned MOD: 2;	// Mode
  unsigned ASP: 2;	// Address stack pointer
  unsigned TTE: 1;	// Tag trasfer enable
  unsigned TIE: 1;	// Tag interrupt enable
  unsigned STR: 1;	// start
  unsigned p1 : 7;
  unsigned TAG:16;	// DMAtag
} sD_CHCR;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

void* Dma::m_saveCurrent = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::waitGif()
{
#ifdef ZENIC_PS2
  int count = 0;
  
  while (*D2_CHCR&0x100)
  {
	//idle to not hammer on the buss too much
	asm __volatile__( "nop;nop;nop;nop;nop;nop;nop;nop;" );
  
    if(count == 500000)
    {
		DmaSimulator sim;

		ZENIC_INFO("gifDma has locked up!");
		ZENIC_INFO((u32)m_saveCurrent);
		sim.start((u32*)m_saveCurrent);
		ctrlStatus();	
		stats();

		u32 Chcr = *D2_CHCR;
		channelStat(2, Chcr);    

		*(volatile char *)0 = 0x666;
		return;
    }
    
    count++;
  }

#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::waitVif1()
{
#ifdef ZENIC_PS2
  int count = 0;
  
  while (*D1_CHCR&0x100)
  {
		//idle to not hammer on the buss too much.
	asm __volatile__( "nop;nop;nop;nop;nop;nop;nop;nop;" );
  
    if (count == 0x1000000)
    {
		//Vu1::dumpOnce("host:vu1micromem","host:vu1mem");

		ZENIC_INFO("vif1Dma has locked up!");
		//u32* kickAdress = (u32*)*D1_TADR;
		DmaSimulator sim;
		ZENIC_INFO((u32)m_saveCurrent);
		sim.start((u32*)m_saveCurrent);
		vif1Stats();
		*(volatile char *)0 = 0x666;
		return;
    }
    
    count++;
  }
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::waitFromSp()
{
#ifdef ZENIC_PS2
	int count = 0;

	while (*D8_CHCR&0x100)
	{
		//idle to not hammer on the buss too much.
		asm __volatile__( "nop;nop;nop;nop;nop;nop;nop;nop;" );

		if (count == 0x100000)
		{
			ZENIC_INFO("fromSpr has locked up!");
			*(volatile char *)0 = 0x666;
			return;
		}

		count++;
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::sendNormalFromSp(void* dest, const void* src, u32 size)
{
#ifdef ZENIC_PS2
	asm("sync.l");
	*D8_MADR = u32(dest);
	*D8_SADR = u32(src);
	*D8_QWC  = size;
	*D8_CHCR = 0x100;
	asm __volatile__("sync.l");
	asm __volatile__("sync.p");    
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::ctrlStatus()
{
  u32 temp2 = *D_CTRL;
  sD_CTRL* p_stat2 = (sD_CTRL*)&temp2;

  logOut << "--------------------------------------------------------------" << endl;
  logOut << "---------------- D_CTRL STATUS (0x%08x) ------------------" << endl;
  logOut << "--------------------------------------------------------------" << endl;
  logOut << "DMAE: DMA enable            " << p_stat2->DMAE << endl;
  logOut << "RELE: release signal enable " << p_stat2->RELE << endl;
  logOut << "MFD: Memory FIFO drain      " << p_stat2->MFD << endl;
  logOut << "STS: Stall source channel   " << p_stat2->STS << endl;
  logOut << "STD: Stall drain channel    " << p_stat2->STD << endl;
  logOut << "RCYC: Release Cycle         " << p_stat2->RCYC << endl;
  printf("D1_MADR                     0x%08x\n", *D1_MADR);
  printf("D1_QWC                      0x%08x\n", *D1_QWC);
  printf("D1_TADR                     0x%08x\n", *D1_TADR);
  printf("D1_ASR0                     0x%08x\n", *D1_ASR0);
  printf("D1_ASR1                     0x%08x\n", *D1_ASR1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::stats()
{
  logOut << "Showing stats" << endl;


  u32 temp = *D_STAT;
  sD_STAT* p_stat = (sD_STAT*)&temp;

  logOut << "--------------------------------------------------------------"  << endl;
  //logOut << "---------------- D_STAT STATUS (0x%08x) ------------------\n", temp );
  logOut << "--------------------------------------------------------------"  << endl;;
  logOut << "CIS0: Channel interrupt status           " << p_stat->CIS0 << endl;
  logOut << "CIS1: Channel interrupt status           " << p_stat->CIS1 << endl;
  logOut << "CIS2: Channel interrupt status           " << p_stat->CIS2 << endl;
  logOut << "CIS3: Channel interrupt status           " << p_stat->CIS3 << endl;
  logOut << "CIS4: Channel interrupt status           " << p_stat->CIS4 << endl;
  logOut << "CIS5: Channel interrupt status           " << p_stat->CIS5 << endl;
  logOut << "CIS6: Channel interrupt status           " << p_stat->CIS6 << endl;
  logOut << "CIS7: Channel interrupt status           " << p_stat->CIS7 << endl;
  logOut << "CIS8: Channel interrupt status           " << p_stat->CIS8 << endl;
  logOut << "CIS9: Channel interrupt status           " << p_stat->CIS9 << endl;
  logOut << "SIS : DAM Stall interrupt status         " << p_stat->SIS << endl;
  logOut << "MEIS: Memory FIFO empty interrupt status " << p_stat->MEIS << endl;
  logOut << "BEIS: BUSERR interrupt status            " << p_stat->BEIS << endl;
  logOut << "CIM0: Channel interrupt mask             " << p_stat->CIM0 << endl;
  logOut << "CIM1: Channel interrupt mask             " << p_stat->CIM1 << endl;
  logOut << "CIM2: Channel interrupt mask             " << p_stat->CIM2 << endl;
  logOut << "CIM3: Channel interrupt mask             " << p_stat->CIM3 << endl;
  logOut << "CIM4: Channel interrupt mask             " << p_stat->CIM4 << endl;
  logOut << "CIM5: Channel interrupt mask             " << p_stat->CIM5 << endl;
  logOut << "CIM6: Channel interrupt mask             " << p_stat->CIM6 << endl;
  logOut << "CIM7: Channel interrupt mask             " << p_stat->CIM7 << endl;
  logOut << "CIM8: Channel interrupt mask             " << p_stat->CIM8 << endl;
  logOut << "CIM9: Channel interrupt mask             " << p_stat->CIM9 << endl;
  logOut << "SIM : DMA Stall interrupt mask           " << p_stat->SIM << endl;
  logOut << "MEIM: Memory FIFO empty interrupt mask   " << p_stat->MEIM << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::vif1Stats()
{
  ctrlStatus();	
  stats();

  u32 temp = *VIF1_STAT;
  vif::sVIF1_STAT* p_stat = (vif::sVIF1_STAT*)&temp;

  // shows stats for vif1 registers
  // notice that this should be moved to vif.cpp or something

  logOut << "--------------------------------------------------------------"  << endl;
  logOut << "--------------------------------------------------------------"  << endl;;
  logOut << "VIF1 pipeline status                     " << p_stat->VPS << endl;
  logOut << "VIF1 E-bit wait                          " << p_stat->VEW << endl;
  logOut << "VIF1 GIF wait                            " << p_stat->VGW << endl;
  logOut << "VIF1 MARK detected flag                  " << p_stat->MRK << endl;
  logOut << "Duble Buffer Flag                        " << p_stat->DBF << endl;
  logOut << "VIF1 stop stall                          " << p_stat->VSS << endl;
  logOut << "VIF1 ForceBreak stall                    " << p_stat->VIS << endl;
  logOut << "VIF1 interrupt stall                     " << p_stat->VIS << endl;
  logOut << "Interrupt bit detected flag              " << p_stat->INT << endl;
  logOut << "Mismatch Error detected flag             " << p_stat->ERO << endl;
  logOut << "Reserved Instruction Error detected flag " << p_stat->ER1 << endl;
  logOut << "VIF1-FIFO direction                      " << p_stat->FDR << endl;
  logOut << "VIF1-FIFO valid data counter             " << p_stat->FQC << endl;

  printf("VIF1_MARK                     0x%08x\n", *VIF1_MARK);
  printf("VIF1_CYCLE                    0x%08x\n", *VIF1_CYCLE);
  printf("VIF1_MODE                     0x%08x\n", *VIF1_MODE);
  printf("VIF1_NUM                      0x%08x\n", *VIF1_NUM);
  printf("VIF1_MASK                     0x%08x\n", *VIF1_MASK);
  printf("VIF1_CODE                     0x%08x\n", *VIF1_CODE);
  printf("VIF1_ITOPS                    0x%08x\n", *VIF1_ITOPS);
  printf("VIF1_BASE                     0x%08x\n", *VIF1_BASE);
  printf("VIF1_OFST                     0x%08x\n", *VIF1_OFST);
  printf("VIF1_TOPS                     0x%08x\n", *VIF1_TOPS);
  printf("VIF1_ITOP                     0x%08x\n", *VIF1_ITOP);
  printf("VIF1_TOP                      0x%08x\n", *VIF1_TOP);

  u32 Chcr = *D1_CHCR;
  channelStat(1, Chcr);    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::channelStat(uint id, u32 Chcr)
{
  u32 pce;   
  u32 id2;   
  u32 irq;   
  
  logOut << "--------------------------------------------------------------" << endl;
  //logOut << "---------------- D%d_CHCR STATUS (0x%08x) -----------------\n", id, Chcr );
  logOut << "--------------------------------------------------------------"  << endl;
  logOut << "DIR: Direction             " << ((Chcr >> 0) & 1) << endl;
  logOut << "MOD: Mode                  " << ((Chcr >> 2) & 3) << endl;
  logOut << "ASP: Address stack pointer " << ((Chcr >> 4) & 3) << endl;
  logOut << "TTE: Tag trasfer enable    " << ((Chcr >> 6) & 1) << endl;
  logOut << "TIE: Tag interrupt enable  " << ((Chcr >> 7) & 1) << endl;
  logOut << "STR: start                 " << ((Chcr >> 8) & 1) << endl;

  ///////////////////////////////////////////////////////////////////////////
  // Get pce,id,irq

  pce = (Chcr >> 26) & 3;
  id2 = (Chcr >> 28) & 7;
  irq = (Chcr >> 31) & 1;
  
  switch( id2 )
  {
    case 1 : logOut << "Last Tag: CNT pce " << pce << " irq " << irq << endl; break;
    case 2 : logOut << "Last Tag: NEXT pce " << pce << " irq " << irq << endl; break;
    case 3 : logOut << "Last Tag: REF pce " << pce << " irq " << irq << endl; break;
    case 5 : logOut << "Last Tag: CALL pce " << pce << " irq " << irq << endl; break;
    case 6 : logOut << "Last Tag: RET pce " << pce << " irq " << irq << endl; break;
    case 7 : logOut << "Last Tag: END pce " << pce << " irq " << irq << endl; break;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef PS2_EMU

void Dma::sendChainVif1(const void* data)
{
//	ZENIC_ASSERT_DESC(0, "Nono..");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::sendChainVif1()
{
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::saveChain(const char* fileName)
{
	File fileOut;
	fileOut.open(fileName, File::Write);
	fileOut.write(m_data, calcSize());
	
	fileOut.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
