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

#ifndef zenic_ps2_SamplingProfiler_h
#define zenic_ps2_SamplingProfiler_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Misc/Profiler/Profiler.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SamplingProfiler
{
public:
	static void singleBegin(const char* name, uint event0, uint event1);
	static void singleEnd();
	static void stats();

	typedef enum 
	{
	  ProcessorCycle = 1,
	  SingleInstructionIssue,
	  BranchIssued,
	  BTACmiss,
	  ITLBmiss,
	  InstructionCacheMiss,
	  AccessToDTLB,
	  NonBlockingLoad,
	  WBBSingleRequest,
	  WBBBurstRequest,
	  CPUAdressBusBusy,
	  InstructionCompleted,
	  NonBDSInstrucionCompleted,
	  COP2InstructionCompleted,
	  LoadCompleted
	} Event0;
	
	typedef enum
	{
	  LowOrderBranchIssued = 0,
	  e1ProcessorCycle,
	  DualInstructionIssue,
	  BranchMispredicted,
	  TLBmiss,
	  DTLBmiss,
	  DataCacheMiss,
	  WBBSingleRequestUnavailable,
	  WBBBurstRequestUnavailable,
	  WBBBurstRequestAlmostFull,
	  WBBBurstRequestFull,
	  CPUDataBusBusy,
	  e1InstructionCompleted,
	  e1NonBDSInstrucionCompleted,
	  COP1InstructionCompleted,
	  StoreCompleted
	} Event1;


private:

	static const char* asString(uint type, uint event);

	typedef struct 
	{
	  u64 p0:1;       
	  // pcr0
	  u64 exl0:1;	// PCR0 operation in Level 1 (0 Not Counted/1 Counted)
	  u64 k0:1;		// PCR0 in Kernel Mode (0 Not Counted/1 Counted)
	  u64 s0:1;		// PCR0 in Supervisor Mode
	  u64 u0:1;		// PCR0 in User Mode
	  u64 event0:5;	// Event to Count by PCR0
	  // pcr1
	  u64 p1:1;		// Always 0
	  u64 exl1:1;	// PCR1 operation in Level 1 (0 Not Counted/1 Counted)
	  u64 k1:1;		// PCR1 in Kernel Mode (0 Not Counted/1 Counted)
	  u64 s1:1;		// PCR1 in Supervisor Mode
	  u64 u1:1;		// PCR1 in User Mode
	  u64 event1:5;	// Event to Count by PCR1
	  u64 p2:11;	// Always 0
	  u64 cte:1;	// Counter Enable (1 Enabled)
	} PccrRegister;

	static const char* m_singleName;
	static u64 m_event0Counter;
	static u64 m_event1Counter;
	static u8 m_event0;
	static u8 m_event1;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ZENIC_PS2_PROFILER_SINGLE_BEGIN(s, event0, event1) zenic::ps2::SamplingProfiler::singleBegin(s, event0, event1)
#define ZENIC_PS2_PROFILER_SINGLE_END() zenic::ps2::SamplingProfiler::singleEnd()
#define ZENIC_PS2_PROFILER_SINGLE_STATS() zenic::ps2::SamplingProfiler::stats()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif
