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

#include "SamplingProfiler.h"
#include <Shared/Base/Io/Log.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* SamplingProfiler::m_singleName = 0;
u64 SamplingProfiler::m_event0Counter = 0;
u64 SamplingProfiler::m_event1Counter = 0;
u8 SamplingProfiler::m_event0 = 0;
u8 SamplingProfiler::m_event1 = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: make inline functions or regular functions

#define setPccr(x)	__asm __volatile (" mtps %0,0 \n\t sync.p" : : "r"(x) : "memory");
#define getPccr(x)	__asm __volatile (" mfps %0,0 " : "=r"(x) :"r"(x) : "memory"); 
#define clearPccr()	__asm __volatile (" mtps $0,0 \n\t sync.p" : : : "memory");
#define clearPc0()	__asm __volatile (" mtpc $0,0 \n\t sync.p" : : : "memory");
#define getPc0(x)	__asm __volatile (" mfpc %0,0 \n\t sync.p" :"=r"(x) :"r"(x) : "memory");
#define clearPc1()	__asm __volatile (" mtpc $0,1 \n\t sync.p" : : : "memory");
#define getPc1(x)	__asm __volatile (" mfpc %0,1 \n\t sync.p" :"=r"(x) :"r"(x) : "memory");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SamplingProfiler::singleBegin(const char* name, uint event0, uint event1)
{
	PccrRegister regTemp;

	m_singleName = name;
	m_event0 = event0;
	m_event1 = event1;

	getPccr(regTemp);

	regTemp.cte = 0;
	regTemp.exl0 = 1;
	regTemp.k0 = 1;
	regTemp.s0 = 1;
	regTemp.u0 = 1;
	
	regTemp.exl1 = 1;
	regTemp.k1 = 1;
	regTemp.s1 = 1;
	regTemp.u1 = 1;
	regTemp.cte = 1;
	regTemp.event0 = event0;
	regTemp.event1 = event1;

	clearPc0();
	clearPc1();

	setPccr(regTemp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SamplingProfiler::singleEnd()
{
	PccrRegister regTemp;

	getPccr(regTemp);
	regTemp.cte = 0;  		// Stop Counters
	setPccr(regTemp);

	getPc0(m_event0Counter);
	clearPc0();	

	getPc1(m_event1Counter);
	clearPc1();	
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SamplingProfiler::stats()
{
	zenic::logOut << "[" << m_singleName << "] " << asString(0, m_event0) << " " << u32(m_event0Counter) 
				 << " " << asString(1, m_event1) << " " << u32(m_event1Counter) << endl << ends << flush;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* SamplingProfiler::asString(uint type, uint event)
{
	if (type == 0)
	{
		switch (event)
		{
			case ProcessorCycle             : return "ProcessorCycle"; 
			case SingleInstructionIssue	    : return "SingleInstructionIssue"; 
			case BranchIssued	            : return "BranchIssued";
			case BTACmiss	                : return "BTACmiss";
			case ITLBmiss	                : return "ITLBmiss"; 
			case InstructionCacheMiss	    : return "InstructionCacheMiss"; 
			case AccessToDTLB	            : return "AccessToDTLB"; 
			case NonBlockingLoad            : return "nonBlockingLoad"; 
			case WBBSingleRequest	        : return "WBBSingleRequest"; 
			case WBBBurstRequest            : return "WBBBurstRequest"; 
			case CPUAdressBusBusy           : return "CPUAdressBusBusy"; 
			case InstructionCompleted	    : return "InstructionCompleted"; 
			case NonBDSInstrucionCompleted  : return "NonBDSInstrucionCompleted"; 
			case COP2InstructionCompleted	: return "COP2InstructionCompleted"; 
			case LoadCompleted              : return "LoadCompleted"; 
		}	

		return "Invalid Event";
	}

	switch (event)
	{
		case LowOrderBranchIssued	     : return "LowOrderBranchIssued";
		case ProcessorCycle	             : return "ProcessorCycle";
		case DualInstructionIssue	     : return "DualInstructionIssue";
		case BranchMispredicted	         : return "BranchMispredicted";
		case TLBmiss                     : return "TLBmiss";
		case DTLBmiss	                 : return "DTLBmiss";
		case DataCacheMiss               : return "DataCacheMiss";
		case WBBSingleRequestUnavailable : return "WBBSingleRequestUnavailable";
		case WBBBurstRequestUnavailable	 : return "WBBBurstRequestUnavailable";
		case WBBBurstRequestAlmostFull	 : return "WBBBurstRequestAlmostFull";
		case WBBBurstRequestFull         : return "WBBBurstRequestFull";
		case CPUDataBusBusy              : return "CPUDataBusBusy";
		case InstructionCompleted        : return "InstructionCompleted";
		case NonBDSInstrucionCompleted	 : return "NonBDSInstrucionCompleted";
		case COP1InstructionCompleted	 : return "COP1InstructionCompleted";
		case StoreCompleted              : return "StoreCompleted";
	}	

	return "Invalid Event";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
