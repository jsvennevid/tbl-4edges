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

#ifndef zenic_ps2_TextureSystem_h
#define zenic_ps2_TextureSystem_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Base/Memory/DataPtr.h>
#include <Shared/Base/Storage/Array.h>
#include <Shared/Hardware/Ps2/Dma.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class Dma;
		class Texture;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TextureSystem
{
public:

	enum Path
	{
		Path2,
		Path3
	};

	TextureSystem(Path path);
	~TextureSystem();

	void create(uint size = 100 * 1024);
	void destroy();

	void setTexture(uint index, const Texture* texture);
	Texture* texture(uint index) const;

	void begin();
	void sync();
	void end();

	void flip();
	void kick();
	void wait();

	void activateTexture(Dma& dmaChain, const Texture* texture, bool alpha = false);

	uint textureOffset();
	void addSync(Dma& dmaChain);

	inline uint offset() { return m_textureOffset;}
	inline uint palOffset() { return m_palOffset;}

private:

	void flush(const Texture* texture = 0);
	void flushPath2(const Texture* texture = 0);
	void flipTextureBuffer();

	//static uint log2(uint value);

	enum { NumNopQwords = 24 }; // number of nop-qwords the VIF should emit on sync 
	enum { TextureMemSize = 512*1024, MaxVideoMemory = 4*1024*1024 };

	Dma m_gifChain;

#ifdef ZENIC_PS2	
	
	u64 m_path3Sync[(3+NumNopQwords)*2] ALIGN(64);
	u64 m_path3Wait[3*2] ALIGN(64);
	u64 m_path3Mask[2] ALIGN(64);

#else

	__declspec(align(16)) u64 m_path3Sync[(3+NumNopQwords)*2] ALIGN(64);
	__declspec(align(16)) u64 m_path3Wait[3*2] ALIGN(64);
	__declspec(align(16)) u64 m_path3Mask[2] ALIGN(64);

#endif

	inline uint calcLog(uint value);

	enum
	{
		MaxTextures = 2
	};

	struct TextureContainer
	{
		u32 vramAddress;
		u32 palAddress;
		const Texture* texture;
	};

	TextureContainer m_textures[MaxTextures];

	u8* m_gifBuffer1;
	u8* m_gifBuffer2;

	uint m_currentTexture;
	uint m_size;
	uint m_gifSize;
	uint m_topTexture;
	uint m_base;
	uint m_currentOffset;
	uint m_textureOffset;
	uint m_palOffset;
	uint m_buffer1;
	uint m_buffer2;
	
	Path m_path;

	bool m_flip;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
