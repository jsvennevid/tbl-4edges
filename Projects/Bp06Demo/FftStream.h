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

#ifndef zenic_ps2_FftStream_h
#define zenic_ps2_FftStream_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Dma;
		class Renderer;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FftStream
{

public:

	FftStream();
	static FftStream& instance();

	void create();
	void update(Renderer& renderer, Dma& dmaChain, bool visualize = false);

	void setTime(float time);

	const u8* kickData() const;
	const u8* hihatData() const;
	const u8* bassData() const;
	const u8* speachData() const;

private:

	u8* loadData(const char* filename);

	u8* m_kickData;
	u8* m_hihatData;
	u8* m_bassData;
	u8* m_speachData;

	uint m_offset;

	static FftStream* s_instance;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const u8* FftStream::kickData() const
{
	if (!m_kickData)
		return 0;

	return static_cast<const u8*>(&m_kickData[m_offset]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const u8* FftStream::hihatData() const
{
	if (!m_hihatData)
		return 0;

	return static_cast<const u8*>(&m_hihatData[m_offset]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const u8* FftStream::bassData() const
{
	if (!m_bassData)
		return 0;

	return static_cast<const u8*>(&m_bassData[m_offset]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const u8* FftStream::speachData() const
{
	if (!m_speachData)
		return 0;

	return static_cast<const u8*>(&m_speachData[m_offset]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif




