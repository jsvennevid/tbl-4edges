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

#ifndef zenic_ps2_TempTexture_h
#define zenic_ps2_TempTexture_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Base/Memory/DataPtr.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TempTexture  : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;

public:
	enum ImageFormat
	{
		CT32	=0x00,
		CT24	= 0x01,
		CT16	= 0x02,
		T8		= 0x13,
		T4		= 0x14
	};

	TempTexture();
	virtual ~TempTexture();

	void serialize(Serializer& s);
	void addToChain(Dma& dmaChain);

	void init();

	void setImageData(u8* data, uint size) { m_imageData = DataPtr<u8>(data, size); }
	u8* imageData() { return m_imageData.objects(); }
	u32 imageSize() { return m_imageData.count(); }

	void setPalData(u8* data, uint size) { m_palData = DataPtr<u8>(data, size); }
	u8* palData() { return m_palData.objects(); }
	u32 palSize() { return m_palData.count(); }

	void setCallChain(u8* data, uint size) { m_callChain = DataPtr<u8>(data, size); }
	u8* callChain() { return m_callChain.objects(); }

	void setPalOffset(u32 offset) { m_palOffset = offset; }
	void setImageOffset(u32 offset) { m_imageOffset = offset; }


private:

	DataPtr<u8>	m_callChain;
	DataPtr<u8>	m_imageData;
	DataPtr<u8>	m_palData;
	
	u32	m_imageOffset;
	u32 m_palOffset;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif //zenic_ps2_Texture_h
