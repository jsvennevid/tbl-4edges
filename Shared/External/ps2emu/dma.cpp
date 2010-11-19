///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "datatypes.h"
#include "vif1.h"
#include "dma.h"
#include "log.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ps2emu
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Dma::Dma() :
	m_buffer(0),
	m_workingBuffer(0),
	m_bufferSize(0),
	m_fileMode(false),
	m_trace(true),
	m_ended(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dma::processAll(Vif1* vif1)
{
	uint i = 0;
	m_ended = false;

	for (i = 0; i != Timeout; ++i)
	{
		if (processTag(vif1))
			break;
	}

	if (i == Timeout)
	{
		Log* log = Log::Instance();
		log->Error(Log::Format("Dma timed out\n"));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Dma::processTag(Vif1* vif1)
{
	u64 tag = *(u64*)m_workingBuffer;
	m_workingBuffer += 2;

	u32 qwc = u32(tag & 0xffff);
	u32 pce = u32((tag >> 26) & 3);
	u32 id = u32((tag >> 28) & 7);
	u32 irq = u32((tag >> 31) & 1);
	u32 addr = u32((tag >> 32) & 0x7fffffff);
	u32 spr = u32((tag >> 63) & 1);

	Log* log = Log::Instance();
	log->SetTraceActive(m_trace);

	// TODO: More verification that the dma data is ok.

	switch(id)
	{
		case Cnt:
		{
			log->Trace(Log::Format("CNT: %d qw (pce = %d, irq = %d, spr = %d)\n", qwc, pce, irq, spr));
			vif1->Read(m_workingBuffer, 2 + (qwc * 4));
			m_workingBuffer += 2 + qwc * 4;
			break;
		}

		case Next:
		{
			log->Trace(Log::Format("NEXT: %08x, %d qw (pce = %d, irq = %d, spr = %d)\n", addr, qwc, pce, irq, spr));
			vif1->Read(m_workingBuffer, 2 + (qwc * 4));
			m_workingBuffer = (u32*)addr;
			break;
		}

		case Ref:
		{
			log->Trace(Log::Format("REF: %08x, %d qw (pce = %d, irq = %d, spr = %d)\n", addr, qwc, pce, irq, spr));
			vif1->Read(m_workingBuffer, 2);
			vif1->Read((u32*)addr, qwc * 4);
			m_workingBuffer += 2;
			break;
		}

		case Call:
		{
			log->Trace(Log::Format("CALL: %08x, %d qw (pce = %d, irq = %d, spr = %d)\n", addr, qwc, pce, irq, spr));
			vif1->Read(m_workingBuffer, 2 + (qwc * 4));
			m_workingBuffer += 2 + (qwc * 4);
			processTag(vif1);
			break;
		}

		case Ret:
		{
			log->Trace(Log::Format("RET: %d qw (pce = %d, irq = %d, spr = %d)\n\n", qwc, pce, irq, spr));
			vif1->Read(m_workingBuffer, 2 + (qwc * 4));
			return m_ended;
			break;
		}

		case End:
		{
			log->Trace(Log::Format("END: %d qw (pce = %d, irq = %d, spr = %d)\n\n", qwc, pce, irq, spr));
			vif1->Read(m_workingBuffer, 2 + (qwc * 4));
			m_ended = true;
			return m_ended;
		}

		default:
		{
			log->Error(Log::Format("DMA id not supported id %d\n", id));
			break;
		}
	}

	log->RestoreTraceState();
	return m_ended;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
