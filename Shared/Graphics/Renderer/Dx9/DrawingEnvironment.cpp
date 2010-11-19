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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DrawingEnvironment.h"

#include <Shared/Base/Debug/Assert.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

namespace win32
{

void DrawingEnvironment::create( IDirect3DDevice9* device )
{
	ZENIC_ASSERT( device );
	m_device = device;
	m_device->AddRef();

	// setup renderstate defaults

	m_renderStates[D3DRS_ZENABLE]				= D3DZB_TRUE;
	m_renderStates[D3DRS_FILLMODE]				= D3DFILL_SOLID;
	m_renderStates[D3DRS_SHADEMODE]				= D3DSHADE_GOURAUD;
	m_renderStates[D3DRS_ZWRITEENABLE]			= TRUE;
	m_renderStates[D3DRS_ALPHATESTENABLE]		= FALSE;
	m_renderStates[D3DRS_LASTPIXEL]				= TRUE;
	m_renderStates[D3DRS_SRCBLEND]				= D3DBLEND_ONE;
	m_renderStates[D3DRS_DESTBLEND]				= D3DBLEND_ZERO;
	m_renderStates[D3DRS_CULLMODE]				= D3DCULL_CCW;
	m_renderStates[D3DRS_ZFUNC]					= D3DCMP_LESSEQUAL;
	m_renderStates[D3DRS_ALPHAREF]				= 0;
	m_renderStates[D3DRS_ALPHAFUNC]				= D3DCMP_ALWAYS;
	m_renderStates[D3DRS_DITHERENABLE]			= FALSE;
	m_renderStates[D3DRS_ALPHABLENDENABLE]		= FALSE;
	m_renderStates[D3DRS_FOGENABLE]				= FALSE;
	m_renderStates[D3DRS_SPECULARENABLE]		= FALSE;
	m_renderStates[D3DRS_FOGCOLOR]				= D3DCOLOR_RGBA(0,0,0,0);
	m_renderStates[D3DRS_FOGTABLEMODE]			= D3DFOG_NONE;
	m_renderStates[D3DRS_FOGSTART]				= 0x00000000; // 0.0f
	m_renderStates[D3DRS_FOGEND]				= 0x3f800000; // 1.0f
	m_renderStates[D3DRS_FOGDENSITY]			= 0x3f800000; // 1.0f
	m_renderStates[D3DRS_RANGEFOGENABLE]		= FALSE;
	m_renderStates[D3DRS_STENCILENABLE]			= FALSE;
	m_renderStates[D3DRS_STENCILFAIL]			= D3DSTENCILOP_KEEP;
	m_renderStates[D3DRS_STENCILZFAIL]			= D3DSTENCILOP_KEEP;
	m_renderStates[D3DRS_STENCILPASS]			= D3DSTENCILOP_KEEP;
	m_renderStates[D3DRS_STENCILFUNC]			= D3DCMP_ALWAYS;
	m_renderStates[D3DRS_STENCILREF]			= 0;
	m_renderStates[D3DRS_STENCILMASK]			= 0xffffffff;
	m_renderStates[D3DRS_STENCILWRITEMASK]		= 0xffffffff;
	m_renderStates[D3DRS_TEXTUREFACTOR]			= 0xffffffff;
	m_renderStates[D3DRS_WRAP0]					= 0;
	m_renderStates[D3DRS_WRAP1]					= 0;
	m_renderStates[D3DRS_WRAP2]					= 0;
	m_renderStates[D3DRS_WRAP3]					= 0;
	m_renderStates[D3DRS_WRAP4]					= 0;
	m_renderStates[D3DRS_WRAP5]					= 0;
	m_renderStates[D3DRS_WRAP6]					= 0;
	m_renderStates[D3DRS_WRAP7]					= 0;
	m_renderStates[D3DRS_CLIPPING]				= TRUE;
	m_renderStates[D3DRS_LIGHTING]				= TRUE;
	m_renderStates[D3DRS_AMBIENT]				= 0;
	m_renderStates[D3DRS_FOGVERTEXMODE]			= D3DFOG_NONE;
	m_renderStates[D3DRS_COLORVERTEX]			= TRUE;
	m_renderStates[D3DRS_LOCALVIEWER]			= TRUE;
	m_renderStates[D3DRS_NORMALIZENORMALS]		= FALSE;
	m_renderStates[D3DRS_DIFFUSEMATERIALSOURCE]	= D3DMCS_COLOR1;
	m_renderStates[D3DRS_SPECULARMATERIALSOURCE]= D3DMCS_COLOR2;
	m_renderStates[D3DRS_AMBIENTMATERIALSOURCE]	= D3DMCS_MATERIAL;
	m_renderStates[D3DRS_EMISSIVEMATERIALSOURCE]= D3DMCS_MATERIAL;
	m_renderStates[D3DRS_VERTEXBLEND]			= D3DVBF_DISABLE;
	m_renderStates[D3DRS_CLIPPLANEENABLE]		= 0;
	m_renderStates[D3DRS_POINTSIZE]				= 0; // TODO: fix this
	m_renderStates[D3DRS_POINTSIZE_MIN]			= 0;
	m_renderStates[D3DRS_POINTSPRITEENABLE]		= FALSE;
	m_renderStates[D3DRS_POINTSCALEENABLE]		= FALSE;
	m_renderStates[D3DRS_POINTSCALE_A]			= 0x3f800000;	// 1.0f
	m_renderStates[D3DRS_POINTSCALE_B]			= 0x00000000;	// 0.0f
	m_renderStates[D3DRS_POINTSCALE_C]			= 0x00000000;	// 0.0f
	m_renderStates[D3DRS_MULTISAMPLEANTIALIAS]	= TRUE;
	m_renderStates[D3DRS_MULTISAMPLEMASK]		= 0xffffffff;
	m_renderStates[D3DRS_PATCHEDGESTYLE]		= D3DPATCHEDGE_DISCRETE;
	m_renderStates[D3DRS_DEBUGMONITORTOKEN]		= D3DDMT_ENABLE;
	m_renderStates[D3DRS_POINTSIZE_MAX]			= 0x42800000;	// 64.0f
	m_renderStates[D3DRS_INDEXEDVERTEXBLENDENABLE]= FALSE;
	m_renderStates[D3DRS_COLORWRITEENABLE]		= 0x0000000f;
	m_renderStates[D3DRS_TWEENFACTOR]			= 0x00000000;	// 0.0f
	m_renderStates[D3DRS_BLENDOP]				= D3DBLENDOP_ADD;
	m_renderStates[D3DRS_POSITIONDEGREE]		= D3DDEGREE_CUBIC;
	m_renderStates[D3DRS_NORMALDEGREE]			= D3DDEGREE_LINEAR;
	m_renderStates[D3DRS_SCISSORTESTENABLE]		= FALSE;
	m_renderStates[D3DRS_SLOPESCALEDEPTHBIAS]	= 0;
	m_renderStates[D3DRS_ANTIALIASEDLINEENABLE]	= FALSE;
	m_renderStates[D3DRS_MINTESSELLATIONLEVEL]	= 0x3f800000;	// 1.0f
	m_renderStates[D3DRS_MAXTESSELLATIONLEVEL]	= 0x3f800000;	// 1.0f
	m_renderStates[D3DRS_ADAPTIVETESS_X]		= 0x00000000;	// 0.0f
	m_renderStates[D3DRS_ADAPTIVETESS_Y]		= 0x00000000;	// 0.0f
	m_renderStates[D3DRS_ADAPTIVETESS_Z]		= 0x3f800000;	// 1.0f
	m_renderStates[D3DRS_ADAPTIVETESS_W]		= 0x00000000;	// 0.0f
	m_renderStates[D3DRS_ENABLEADAPTIVETESSELLATION]	= FALSE;
	m_renderStates[D3DRS_TWOSIDEDSTENCILMODE]	= FALSE;
	m_renderStates[D3DRS_CCW_STENCILFAIL]		= D3DSTENCILOP_KEEP;
	m_renderStates[D3DRS_CCW_STENCILZFAIL]		= D3DSTENCILOP_KEEP;
	m_renderStates[D3DRS_CCW_STENCILPASS]		= D3DSTENCILOP_KEEP;
	m_renderStates[D3DRS_CCW_STENCILFUNC]		= D3DCMP_ALWAYS;
	m_renderStates[D3DRS_COLORWRITEENABLE1]		= 0x0000000f;
	m_renderStates[D3DRS_COLORWRITEENABLE2]		= 0x0000000f;
	m_renderStates[D3DRS_COLORWRITEENABLE3]		= 0x0000000f;
	m_renderStates[D3DRS_BLENDFACTOR]			= 0xffffffff;
	m_renderStates[D3DRS_SRGBWRITEENABLE]		= 0;
	m_renderStates[D3DRS_DEPTHBIAS]				= 0;
	m_renderStates[D3DRS_WRAP8]					= 0;
	m_renderStates[D3DRS_WRAP9]					= 0;
	m_renderStates[D3DRS_WRAP10]				= 0;
	m_renderStates[D3DRS_WRAP11]				= 0;
	m_renderStates[D3DRS_WRAP12]				= 0;
	m_renderStates[D3DRS_WRAP13]				= 0;
	m_renderStates[D3DRS_WRAP14]				= 0;
	m_renderStates[D3DRS_WRAP15]				= 0;
	m_renderStates[D3DRS_SEPARATEALPHABLENDENABLE]= FALSE;
	m_renderStates[D3DRS_SRCBLENDALPHA]			= D3DBLEND_ONE;
	m_renderStates[D3DRS_DESTBLENDALPHA]		= D3DBLEND_ZERO;
	m_renderStates[D3DRS_BLENDOPALPHA]			= D3DBLENDOP_ADD;

	// texturestagestates

	for( uint i = 0; i < TextureStages; ++i )
	{
		m_textureStages[i][D3DTSS_COLOROP]			= i ? D3DTOP_DISABLE : D3DTOP_MODULATE;
		m_textureStages[i][D3DTSS_COLORARG1]		= D3DTA_TEXTURE;
		m_textureStages[i][D3DTSS_COLORARG2]		= D3DTA_CURRENT;
		m_textureStages[i][D3DTSS_ALPHAOP]			= i ? D3DTOP_DISABLE : D3DTOP_SELECTARG1;
		m_textureStages[i][D3DTSS_ALPHAARG1]		= D3DTA_TEXTURE;
		m_textureStages[i][D3DTSS_ALPHAARG2]		= D3DTA_CURRENT;
		m_textureStages[i][D3DTSS_BUMPENVMAT00]		= 0x00000000;	// 0.0f
		m_textureStages[i][D3DTSS_BUMPENVMAT01]		= 0x00000000;	// 0.0f
		m_textureStages[i][D3DTSS_BUMPENVMAT10]		= 0x00000000;	// 0.0f
		m_textureStages[i][D3DTSS_BUMPENVMAT11]		= 0x00000000;	// 0.0f
		m_textureStages[i][D3DTSS_TEXCOORDINDEX]	= i;
		m_textureStages[i][D3DTSS_BUMPENVLSCALE]	= 0x00000000;	// 0.0f
		m_textureStages[i][D3DTSS_BUMPENVLOFFSET]	= 0x00000000;	// 0.0f
		m_textureStages[i][D3DTSS_TEXTURETRANSFORMFLAGS] = D3DTTFF_DISABLE;
		m_textureStages[i][D3DTSS_COLORARG0]		= D3DTA_CURRENT;
		m_textureStages[i][D3DTSS_ALPHAARG0]		= D3DTA_CURRENT;
		m_textureStages[i][D3DTSS_RESULTARG]		= D3DTA_CURRENT;
		m_textureStages[i][D3DTSS_CONSTANT]			= 0;
	}

	// samplerstate

	for( uint i = 0; i < Samplers; ++i )
	{
		m_samplers[i][D3DSAMP_ADDRESSU]		= D3DTADDRESS_WRAP;
		m_samplers[i][D3DSAMP_ADDRESSV]		= D3DTADDRESS_WRAP;
		m_samplers[i][D3DSAMP_ADDRESSW]		= D3DTADDRESS_WRAP;
		m_samplers[i][D3DSAMP_BORDERCOLOR]	= D3DCOLOR_RGBA(0,0,0,0);
		m_samplers[i][D3DSAMP_MAGFILTER]	= D3DTEXF_POINT;
		m_samplers[i][D3DSAMP_MINFILTER]	= D3DTEXF_POINT;
		m_samplers[i][D3DSAMP_MIPFILTER]	= D3DTEXF_NONE;
		m_samplers[i][D3DSAMP_MIPMAPLODBIAS]= 0;
		m_samplers[i][D3DSAMP_MAXANISOTROPY]= 1;
		m_samplers[i][D3DSAMP_SRGBTEXTURE]	= 0;
		m_samplers[i][D3DSAMP_ELEMENTINDEX]	= 0;
		m_samplers[i][D3DSAMP_DMAPOFFSET]	= 0;
	}

	// render targets

	for( uint i = 0; i < RenderTargets; ++i )
		m_renderTargets[i] = 0;

	// textures

	for( uint i = 0; i < Textures; ++i )
		m_textures[i] = 0;
}

void DrawingEnvironment::destroy()
{
	// drop all rendertargets

	for( uint i = 0; i < RenderTargets; ++i )
		setRenderTarget( i, 0 );

	// drop all textures

	for( uint i = 0; i < Textures; ++i )
		setTexture( i, 0 );

	if( m_device )
	{
		m_device->Release();
		m_device = 0;
	}
}

}

}
