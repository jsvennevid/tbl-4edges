#include "GsRenderer.h"
#include "BufferStream.h"
#include "Log.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ps2emu
{

GsRenderer::GsRenderer(bool trace) : 
	m_trace(trace), 
	m_mode(Tag)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GsRenderer::callback(void* userData, const void* gifData)
{
	GsRenderer* renderer = reinterpret_cast<GsRenderer*>(userData);
	renderer->process(reinterpret_cast<const u32*>(gifData));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GsRenderer::readTag(BufferStream& bufferStream)
{
	u32 tag1 = bufferStream.read<u32>();
	u32 tag2 = bufferStream.read<u32>();
	u64 tag3 = bufferStream.read<u64>();

	m_tag.nloop = (tag1 >> 0) & 0x7fff;
	m_tag.eop = (tag1 >> 15) & 0x1;
	m_tag.pre = (tag2 >> 14) & 0x1;
	m_tag.prim = (tag2 >> 15) & 0x7ff;
	m_tag.flg = (tag2 >> 26) & 0x3;
	m_tag.nreg = (tag2 >> 28) & 0xf;
	m_tag.regs = (u32)tag3;

	if (m_tag.nreg == 0) m_tag.nreg = 16;

	Log* log = Log::Instance();
	log->Trace(Log::Format("gifmode=%x giftag[nloop=%x eop=%x pre=%x prim=%x flg=%x nreg=%x regs=%x_%x]\n",
							m_mode, m_tag.nloop, m_tag.eop, m_tag.pre, m_tag.prim, m_tag.flg, m_tag.nreg,
							((u32*)&m_tag.regs+1), ((u32*)&m_tag.regs+1)));


	m_mode = static_cast<Mode>((m_tag.flg + 1));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GsRenderer::process(const void* gifData)
{
	BufferStream stream(gifData, 1000000);

	Log* log = Log::Instance();
	log->SetTraceActive(m_trace);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	readTag(stream);

	switch (m_mode)
	{

		case Packed :
		{
			if (m_tag.pre)
				prim(m_tag.prim);

			// Really ok to do this here?

			switch (m_prim.prim)
			{
				case Triangle : glBegin(GL_TRIANGLES); break;
			}

			for (uint i = 0; i < m_tag.nloop; ++i)
			{
				for (uint regCount = 0; regCount < m_tag.nreg; ++regCount)
				{
					u32 reg = (m_tag.regs >> (regCount << 2)) & 0xf;
					processReg(reg, stream);	
				}
			}

			glEnd();

			break;
		}

		case Reglist :
		{

			break;
		}

		case Image :
		{

			break;
		}
	}

	m_mode = Tag;

	log->RestoreTraceState();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GsRenderer::processReg(u32 reg, BufferStream& stream)
{
	switch (reg)
	{
		case 0x1 : // Rgba
		{
			glColor4b(stream.read<u32>(), stream.read<u32>(), stream.read<u32>(), stream.read<u32>());
			break;
		}

		// TODO: Handle drawing kick

		case 0x5 : // Xyz2
		{
			glColor3f(1.0f, 1.0f, 1.0f);

			float x = stream.read<u32>() * 1.0f / 16.0f; // Preserve subpixels like some morons dont.
			float y = stream.read<u32>() * 1.0f / 16.0f; // Preserve subpixels like some morons dont.
			float z = stream.read<u32>() * 0.000001f;     // fix me
			u32 adc = stream.read<u32>();

			// TODO: Handle proper xf_offsets

			y -= 2048.0f;
			x -= 2048.0f;

			glVertex3f(x, y, -z);

			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GsRenderer::prim(u32 prim)
{
	m_prim.prim = (Primitive)((prim) & 0x7);
	m_prim.iip  = (prim>>3) & 0x1;
	m_prim.tme  = (prim>>4) & 0x1;
	m_prim.fge  = (prim>>5) & 0x1;
	m_prim.abe  = (prim>>6) & 0x1;
	m_prim.aa1  = (prim>>7) & 0x1;
	m_prim.fst  = (prim>>8) & 0x1;
	m_prim.ctxt = (prim>>9) & 0x1;
	m_prim.fix  = (prim>>10) & 0x1;

	if (m_prim.iip)
		glShadeModel(GL_SMOOTH); 
	else 
		glShadeModel(GL_FLAT);
}



}