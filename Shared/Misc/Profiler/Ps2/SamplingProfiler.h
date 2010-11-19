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
