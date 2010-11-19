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

#ifndef zenic_MasterChain_h
#define zenic_MasterChain_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Storage/Singleton.h>
#include <Shared/Base/Types.h>
#include "Dma.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MasterChain : public Singleton<MasterChain>
{
public:

	MasterChain(uint size = 200 * 1024);	// default 200KB of mem.
	~MasterChain();

	Dma& chain();

	void wait();
	void kick();
	void flip();

private:

	u8* m_dmaBuffer1;
	u8* m_dmaBuffer2;

	u32	m_size;
	bool m_flip;

	Dma m_vif1Chain;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void MasterChain::wait()
{
	Dma::waitVif1();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Dma& MasterChain::chain()
{
	return m_vif1Chain;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif 
