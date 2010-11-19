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

#ifndef zenic_win32_DrawingEnvironment_h
#define zenic_win32_DrawingEnvironment_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Base/Debug/Assert.h>

#if defined(ZENIC_WIN32)
#include <d3d9.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// forwards
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IDirect3DDevice9;

namespace zenic
{

namespace win32
{
	class Texture;
	class RenderTarget;
	class Shader;
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

namespace win32
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DrawingEnvironment
{
public:

	enum
	{
		RenderStates = 210,
		RenderStateQueueSize = 32,
		RenderTargets = 4,

		TextureStages = 8,
		TextureStageStates = 33,

		Samplers = 16,
		SamplerStates = 14,

		Textures = 16
	};

	DrawingEnvironment();
	~DrawingEnvironment();

	void create( IDirect3DDevice9* device );
	void destroy();

	void setRenderState( uint state, u32 value );
	void setFloatRenderState( uint state, f32 value );
	u32 renderState( uint state );

	void setTextureStageState( uint stage, uint state, u32 value );
	u32 textureStageState( uint stage, uint state );

	void setSamplerState( uint sampler, uint state, u32 value );
	u32 samplerState( uint sampler, uint state );

	void setRenderTarget( uint index, RenderTarget* target );
	RenderTarget* renderTarget( uint index );

	void setTexture( uint stage, Texture* texture );
	Texture* texture( uint stage );

private:

	u32 m_renderStates[RenderStates];

	u32 m_textureStages[TextureStages][TextureStageStates];
	u32 m_samplers[Samplers][SamplerStates];

	Texture* m_textures[Textures];
	RenderTarget* m_renderTargets[RenderTargets];

	IDirect3DDevice9* m_device;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DrawingEnvironment.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

}

#endif
